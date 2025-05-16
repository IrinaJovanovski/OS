.global _ZN3TCB13contextSwitchEPNS_7ContextES1_
.type _ZN3TCB13contextSwitchEPNS_7ContextES1_, @function
_ZN3TCB13contextSwitchEPNS_7ContextES1_:
    sd ra, 0 * 8(a0)  // old->context.ra = ra;
    sd sp, 1 * 8(a0)  // old->context.sp = sp;

    ld ra, 0 * 8(a1)  // ra = running->context.ra;
    ld sp, 1 * 8(a1)  // sp = running->context.sp;


    ret