/* < DTS2013093005579 xiaohui 20131009 begin */

void main()
{
#ifdef NeXT
    printf("nextstep\n");
    exit(0);
#endif
    printf("unknown\n");
    exit(0);/*lint !e718*/
}
/* DTS2013093005579 xiaohui 20131009 end > */


