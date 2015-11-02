/*
 * YAFFS: Yet Another Flash File System. A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2011 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
/**********************问题单修改记录******************************************
日    期              修改人         问题单号                   修改内容
******************************************************************************/
#include "yaffs_nand.h"
#include "yaffs_tagscompat.h"

#include "yaffs_getblockinfo.h"
#include "yaffs_summary.h"

#include "linux/mtd/mtd.h"
#include "yaffs_linux.h"

int yaffs_add_prior_gc_chunk(struct yaffs_dev *dev, int nand_chunk, unsigned obj_id)
{
    struct yaffs_gc_chunks *gc_chunk = NULL;
    struct yaffs_gc_chunks *p_gc_chunk = NULL;

    if (!dev->param.scan_ecc_enable)
        return 0;

    /* If obj_id:0 this is not used */
    if ((obj_id == 0) ||
            obj_id == YAFFS_OBJECTID_CHECKPOINT_DATA)
    {
        return 0;
    }

    yaffs_trace(YAFFS_TRACE_ERROR, "---yaffs_add_prior_gc_chunk---");
    p_gc_chunk = dev->prior_gc_chunk_head;
    while (p_gc_chunk)
    {
        if (nand_chunk == p_gc_chunk->chunk_id)
        {
            yaffs_trace(YAFFS_TRACE_ALWAYS, "The chunk has already in the gc  %d", nand_chunk);
            return 0;
        }

        p_gc_chunk = p_gc_chunk->next;
    }

    gc_chunk = (struct yaffs_gc_chunks *)kmalloc(sizeof(struct yaffs_gc_chunks), GFP_KERNEL);
    if (!gc_chunk)
    {
        yaffs_trace(YAFFS_TRACE_ALWAYS, "malloc gc_chunk failed!");
        return - ENOMEM;
    }

    gc_chunk->chunk_id = nand_chunk;
    gc_chunk->obj_id = obj_id;
    gc_chunk->next = NULL;

    if (!dev->prior_gc_chunk_head)
    {
        dev->prior_gc_chunk_head = gc_chunk;
    }
    else
    {
        p_gc_chunk = dev->prior_gc_chunk_head;
        while (p_gc_chunk->next)
        {
            p_gc_chunk = p_gc_chunk->next;
        }
        p_gc_chunk->next = gc_chunk;
    }

    yaffs_trace(YAFFS_TRACE_ALWAYS,
                "add chunk success, chunk id [%d] obj id [%d]",
                gc_chunk->chunk_id, gc_chunk->obj_id);

    return 0;
}

void yaffs_free_prior_gc_chunk(struct yaffs_dev *dev)
{
    struct yaffs_gc_chunks *gc_chunk = NULL;

    if (NULL == dev)
    {
        yaffs_trace(YAFFS_TRACE_ALWAYS,
                    "dev null, free prior gc chunk failed!");
        return;
    }

    if (!dev->param.scan_ecc_enable)
        return;

    while (dev->prior_gc_chunk_head)
    {
        gc_chunk = dev->prior_gc_chunk_head;
        dev->prior_gc_chunk_head = dev->prior_gc_chunk_head->next;
        kfree(gc_chunk);
    }

    dev->prior_gc_chunk_head = NULL;
}
int yaffs_rd_chunk_tags_nand(struct yaffs_dev *dev, int nand_chunk,
			     u8 *buffer, struct yaffs_ext_tags *tags)
{
	int result;
	struct yaffs_ext_tags local_tags;
        struct mtd_info *mtd = yaffs_dev_to_mtd(dev);
	int flash_chunk = nand_chunk - dev->chunk_offset;

	dev->n_page_reads++;

	/* If there are no tags provided use local tags. */
	if (!tags)
		tags = &local_tags;

	if (dev->param.read_chunk_tags_fn)
		result =
		    dev->param.read_chunk_tags_fn(dev, flash_chunk, buffer,
						  tags);
	else
		result = yaffs_tags_compat_rd(dev,
					      flash_chunk, buffer, tags);
	if (tags && tags->ecc_result > YAFFS_ECC_RESULT_NO_ERROR) {

		struct yaffs_block_info *bi;
		bi = yaffs_get_block_info(dev,
					  nand_chunk /
					  dev->param.chunks_per_block);
		yaffs_handle_chunk_error(dev, bi);
	}
        if (mtd->exceed_threshold && tags && tags->ecc_result == YAFFS_ECC_RESULT_FIXED)
        {
            yaffs_trace(YAFFS_TRACE_MTD, "exceed_threshold is marked");
            yaffs_add_prior_gc_chunk(dev, nand_chunk, tags->obj_id);
        }
	return result;
}

int yaffs_wr_chunk_tags_nand(struct yaffs_dev *dev,
				int nand_chunk,
				const u8 *buffer, struct yaffs_ext_tags *tags)
{
	int result;
	int flash_chunk = nand_chunk - dev->chunk_offset;

	dev->n_page_writes++;

	if (tags) {
		tags->seq_number = dev->seq_number;
		tags->chunk_used = 1;
		yaffs_trace(YAFFS_TRACE_WRITE,
			"Writing chunk %d tags %d %d",
			nand_chunk, tags->obj_id, tags->chunk_id);
	} else {
		yaffs_trace(YAFFS_TRACE_ERROR, "Writing with no tags");
		BUG();
		return YAFFS_FAIL;
	}

	if (dev->param.write_chunk_tags_fn)
		result = dev->param.write_chunk_tags_fn(dev, flash_chunk,
							buffer, tags);
	else
		result = yaffs_tags_compat_wr(dev, flash_chunk, buffer, tags);

	yaffs_summary_add(dev, tags, nand_chunk);

	return result;
}

int yaffs_mark_bad(struct yaffs_dev *dev, int block_no)
{
	block_no -= dev->block_offset;
	if (dev->param.bad_block_fn)
		return dev->param.bad_block_fn(dev, block_no);

	return yaffs_tags_compat_mark_bad(dev, block_no);
}

int yaffs_query_init_block_state(struct yaffs_dev *dev,
				 int block_no,
				 enum yaffs_block_state *state,
				 u32 *seq_number)
{
	block_no -= dev->block_offset;
	if (dev->param.query_block_fn)
		return dev->param.query_block_fn(dev, block_no, state,
						 seq_number);

	return yaffs_tags_compat_query_block(dev, block_no, state, seq_number);
}

int yaffs_erase_block(struct yaffs_dev *dev, int flash_block)
{
	int result;

	flash_block -= dev->block_offset;
	dev->n_erasures++;
	result = dev->param.erase_fn(dev, flash_block);
	return result;
}

int yaffs_init_nand(struct yaffs_dev *dev)
{
	if (dev->param.initialise_flash_fn)
		return dev->param.initialise_flash_fn(dev);
	return YAFFS_OK;
}
