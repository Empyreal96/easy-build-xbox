
VOID
BldrEncoderTranslate(
    UCHAR i0, 
    UCHAR i1, 
    UCHAR i2, 
    UCHAR i3,
    UCHAR* o0,
    UCHAR* o1
    )
{
	//
	// COMPLEX: This code is bogus, and the real version is missing.  But we
	// can replace it from Cromwell: cromwell/boot_rom/2bPicResponseAction.c
	// in function BootPicManipulation.
	//
#if 0

    *o0 = i0 + i1;
    *o1 = i2 + i3;

#else

	UCHAR b1, b2, b3, b4;
	ULONG n;

	b1 = 0x33;
	b2 = 0xED;
	b3 = (i0 << 2) ^ (i1 + 0x39) ^ (i2 >> 2) ^ (i3 + 0x63);
	b4 = (i0 + 0x0B) ^ (i1 >> 2) ^ (i2 + 0x1B);

	for (n = 0; n < 4; n++) {
		b1 += b2 ^ b3;
		b2 += b1 ^ b4;
	}

	*o0 = b1;
	*o1 = b2;

#endif
}

