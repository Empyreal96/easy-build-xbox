//
// NOTE: Checked in version of this file should only contain the test keys
// The checked in keys are intended to be used for DevKits (xm2) only.
//


//
// Encryption key used to encrypt the boot loader.  We only use this when
// reloading a media ROM
//

#ifdef RETAILXM3

const UCHAR KeyToDecryptBldr[] =
    {
        0x27,0x45,0xa9,0x10,0x39,0x7e,0x6a,0xa6,
        0x86,0xfb,0x4b,0x1a,0x4b,0xa9,0x0f,0xd2
    };

#else

const UCHAR KeyToDecryptBldr[] =
    {
        0x57,0x42,0x29,0x0C,0x30,0x1E,0xD3,0x01,
        0xB3,0xE5,0x5D,0x28,0x50,0x31,0xE1,0xCE
    };

#endif

//
// This array contains multiple 16-byte decryption keys:
//  1. The first key is used to decrypt the encrypted section of the EEPROM.
//  2. The second key is the CERT key.
//  3. Encryption key used to encrypt/decrypt the kernel
//

#ifdef RETAILXM3

const UCHAR XboxCryptKeys[3 * XBOX_KEY_LENGTH] = {
    // EEPROM key
    0x2a, 0x3b, 0xad, 0x2c, 0xb1, 0x94, 0x4f, 0x93,
    0xaa, 0xcd, 0xcd, 0x7e, 0x0a, 0xc2, 0xee, 0x5a,

    // CERT key
    0x5c, 0x07, 0x33, 0xae, 0x04, 0x01, 0xf7, 0xe8,
    0xba, 0x79, 0x93, 0xfd, 0xcd, 0x2f, 0x1f, 0xe0,

    // Kernel encryption key
    0xee, 0x00, 0x6d, 0x2c, 0x35, 0x37, 0x2d, 0x9a,
    0xd3, 0x92, 0x27, 0xfb, 0xf4, 0x1f, 0xec, 0x20

};

#else

const UCHAR XboxCryptKeys[3 * XBOX_KEY_LENGTH] = {
    // EEPROM key
    0x7b, 0x35, 0xa8, 0xb7, 0x27, 0xed, 0x43, 0x7a,
    0xa0, 0xba, 0xfb, 0x8f, 0xa4, 0x38, 0x61, 0x80,

    // CERT key
    0x66, 0x81, 0x0d, 0x37, 0x91, 0xfd, 0x45, 0x7f,
    0xbf, 0xa9, 0x76, 0xf8, 0xa4, 0x46, 0xa4, 0x94,

    // Kernel encryption key
    0xAD, 0x32, 0x64, 0x01, 0x2F, 0xC8, 0xF4, 0xAD,  
    0xA9, 0xF5, 0xAF, 0x45, 0x22, 0xB7, 0x18, 0xD1   

};

#endif