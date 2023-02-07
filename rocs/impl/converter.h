/*
 Rocs - OS independent C library

 Copyright (C) 2002-2007 - Rob Versluis <r.j.versluis@rocrail.net>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public License
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef __ROCS_CONVERTER_H
#define __ROCS_CONVERTER_H


static const unsigned char Cp1252_AsciiToEbcdicTable [256] =
{
  /* Codepage 1252 -> EBCDIC */
/*       0     1     2     3     4     5     6     7         8     9     A     B     C     D     E      F*/
/* 0*/ 0x00, 0x01, 0x02, 0x03, 0x37, 0x2D, 0x2E, 0x2F,      0x16, 0x05, 0x25, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
/* 1*/ 0x10, 0x11, 0x12, 0x13, 0x3C, 0x3D, 0x32, 0x26,      0x18, 0x19, 0x3F, 0x27, 0x1C, 0x1D, 0x1E, 0x1F,
/* 2*/ 0x40, 0x4F, 0x7F, 0x7B, 0x5B, 0x6C, 0x50, 0x7D,      0x4D, 0x5D, 0x5C, 0x4E, 0x6B, 0x60, 0x4B, 0x61,
/* 3*/ 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,      0xF8, 0xF9, 0x7A, 0x5E, 0x4C, 0x7E, 0x6E, 0x6F,
/* 4*/ 0x7C, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,      0xC8, 0xC9, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6,
/* 5*/ 0xD7, 0xD8, 0xD9, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6,      0xE7, 0xE8, 0xE9, 0x63, 0xEC, 0xFC, 0x5F, 0x6D,
/* 6*/ 0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,      0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
/* 7*/ 0x97, 0x98, 0x99, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6,      0xA7, 0xA8, 0xA9, 0x43, 0xBB, 0xDC, 0xA1, 0x07,
/* 8*/ 0x20, 0x21, 0x22, 0x23, 0x24, 0x15, 0x06, 0x17,      0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x09, 0x0A, 0x1B,
/* 9*/ 0x30, 0x31, 0x1A, 0x33, 0x34, 0x35, 0x36, 0x08,      0x38, 0x39, 0x3A, 0x3B, 0x04, 0x14, 0x3E, 0xFF,
/* A*/ 0x41, 0xAA, 0xB0, 0xB1, 0x9F, 0xB2, 0xCC, 0xB5,      0xBD, 0xB4, 0x9A, 0x8A, 0xBA, 0xCA, 0xAF, 0xBC,
/* B*/ 0x90, 0x8F, 0xEA, 0xFA, 0xBE, 0xA0, 0xB6, 0xB3,      0x9D, 0xDA, 0x9B, 0x8B, 0xB7, 0xB8, 0xB9, 0xAB,
/* C*/ 0x64, 0x65, 0x62, 0x66, 0x4A, 0x67, 0x9E, 0x68,      0x74, 0x71, 0x72, 0x73, 0x78, 0x75, 0x76, 0x77,
/* D*/ 0xAC, 0x69, 0xED, 0xEE, 0xEB, 0xEF, 0xE0, 0xBF,      0x80, 0xFD, 0xFE, 0xFB, 0x5A, 0xAD, 0xAE, 0x59,
/* E*/ 0x44, 0x45, 0x42, 0x46, 0xC0, 0x47, 0x9C, 0x48,      0x54, 0x51, 0x52, 0x53, 0x58, 0x55, 0x56, 0x57,
/* F*/ 0x8C, 0x49, 0xCD, 0xCE, 0xCB, 0xCF, 0x6A, 0xE1,      0x70, 0xDD, 0xDE, 0xDB, 0xD0, 0x8D, 0x8E, 0xDF,
};

static const unsigned char Cp1252_EbcdicToAsciiTable [256] =
{
  /* EBCDIC -> Codepage 1252 */
/*       0     1     2     3     4     5     6     7         8     9     A     B     C     D     E      F*/
/* 0*/ 0x00, 0x01, 0x02, 0x03, 0x9C, 0x09, 0x86, 0x7F,      0x97, 0x8D, 0x8E, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
/* 1*/ 0x10, 0x11, 0x12, 0x13, 0x9D, 0x85, 0x08, 0x87,      0x18, 0x19, 0x92, 0x8F, 0x1C, 0x1D, 0x1E, 0x1F,
/* 2*/ 0x80, 0x81, 0x82, 0x83, 0x84, 0x0A, 0x17, 0x1B,      0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x05, 0x06, 0x07,
/* 3*/ 0x90, 0x91, 0x16, 0x93, 0x94, 0x95, 0x96, 0x04,      0x98, 0x99, 0x9A, 0x9B, 0x14, 0x15, 0x9E, 0x1A,
/* 4*/ 0x20, 0xA0, 0xE2, 0x7B, 0xE0, 0xE1, 0xE3, 0xE5,      0xE7, 0xF1, 0xC4, 0x2E, 0x3C, 0x28, 0x2B, 0x21,
/* 5*/ 0x26, 0xE9, 0xEA, 0xEB, 0xE8, 0xED, 0xEE, 0xEF,      0xEC, 0xDF, 0xDC, 0x24, 0x2A, 0x29, 0x3B, 0x5E,
/* 6*/ 0x2D, 0x2F, 0xC2, 0x5B, 0xC0, 0xC1, 0xC3, 0xC5,      0xC7, 0xD1, 0xF6, 0x2C, 0x25, 0x5F, 0x3E, 0x3F,
/* 7*/ 0xF8, 0xC9, 0xCA, 0xCB, 0xC8, 0xCD, 0xCE, 0xCF,      0xCC, 0x60, 0x3A, 0x23, 0x40, 0x27, 0x3D, 0x22,
/* 8*/ 0xD8, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,      0x68, 0x69, 0xAB, 0xBB, 0xF0, 0xFD, 0xFE, 0xB1,
/* 9*/ 0xB0, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,      0x71, 0x72, 0xAA, 0xBA, 0xE6, 0xB8, 0xC6, 0xA4,
/* A*/ 0xB5, 0x7E, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,      0x79, 0x7A, 0xA1, 0xBF, 0xD0, 0xDD, 0xDE, 0xAE,
/* B*/ 0xA2, 0xA3, 0xA5, 0xB7, 0xA9, 0xA7, 0xB6, 0xBC,      0xBD, 0xBE, 0xAC, 0x7C, 0xAF, 0xA8, 0xB4, 0xD7,
/* C*/ 0xE4, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,      0x48, 0x49, 0xAD, 0xF4, 0xA6, 0xF2, 0xF3, 0xF5,
/* D*/ 0xFC, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,      0x51, 0x52, 0xB9, 0xFB, 0x7D, 0xF9, 0xFA, 0xFF,
/* E*/ 0xD6, 0xF7, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,      0x59, 0x5A, 0xB2, 0xD4, 0x5C, 0xD2, 0xD3, 0xD5,
/* F*/ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,      0x38, 0x39, 0xB3, 0xDB, 0x5D, 0xD9, 0xDA, 0x9F,
};

static const unsigned char Cp437_AsciiToEbcdicTable [256] =
{
  /* Codepage 437/850 -> EBCDIC */
/*       0     1     2     3     4     5     6     7         8     9     A     B     C     D     E      F*/
/* 0*/ 0x00, 0x01, 0x02, 0x03, 0x37, 0x2D, 0x2E, 0x2F,      0x16, 0x05, 0x25, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
/* 1*/ 0x10, 0x11, 0x12, 0x13, 0x3C, 0x3D, 0x32, 0x26,      0x18, 0x19, 0x3F, 0x27, 0x1C, 0x1D, 0x1E, 0x1F,
/* 2*/ 0x40, 0x4F, 0x7F, 0x7B, 0x5B, 0x6C, 0x50, 0x7D,      0x4D, 0x5D, 0x5C, 0x4E, 0x6B, 0x60, 0x4B, 0x61,
/* 3*/ 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,      0xF8, 0xF9, 0x7A, 0x5E, 0x4C, 0x7E, 0x6E, 0x6F,
/* 4*/ 0x7C, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,      0xC8, 0xC9, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6,
/* 5*/ 0xD7, 0xD8, 0xD9, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6,      0xE7, 0xE8, 0xE9, 0x63, 0xEC, 0xFC, 0x5F, 0x6D,
/* 6*/ 0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,      0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
/* 7*/ 0x97, 0x98, 0x99, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6,      0xA7, 0xA8, 0xA9, 0x43, 0xBB, 0xDC, 0xA1, 0x07,
/* 8*/ 0x68, 0xD0, 0x51, 0x42, 0xC0, 0x44, 0x47, 0x48,      0x52, 0x53, 0x54, 0x57, 0x56, 0x58, 0x4A, 0x67,
/* 9*/ 0x71, 0x9C, 0x9E, 0xCB, 0x6A, 0xCD, 0xDB, 0xDD,      0xDF, 0xE0, 0x5A, 0x70, 0xB1, 0x80, 0xBF, 0x23,
/* A*/ 0x45, 0x55, 0xCE, 0xDE, 0x49, 0x69, 0x9A, 0x9B,      0xAB, 0xAF, 0xBA, 0xB8, 0xB7, 0xAA, 0x8A, 0x8B,
/* B*/ 0x30, 0x31, 0x1A, 0x33, 0x34, 0x65, 0x62, 0x64,      0xB4, 0x39, 0x3A, 0x3B, 0x04, 0xB0, 0xB2, 0xFF,
/* C*/ 0x14, 0x21, 0x22, 0x24, 0x36, 0x06, 0x46, 0x66,      0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x08, 0x17, 0x9F,
/* D*/ 0x8C, 0xAC, 0x72, 0x73, 0x74, 0x20, 0x75, 0x76,      0x77, 0x09, 0x0A, 0x1B, 0x3E, 0xCC, 0x78, 0x38,
/* E*/ 0xEE, 0x59, 0xEB, 0xED, 0xCF, 0xEF, 0xA0, 0x8E,      0xAE, 0xFE, 0xFB, 0xFD, 0x8D, 0xAD, 0xBC, 0xBE,
/* F*/ 0xCA, 0x8F, 0x15, 0xB9, 0xB6, 0xB5, 0xE1, 0x9D,      0x90, 0xBD, 0xB3, 0xDA, 0xFA, 0xEA, 0x35, 0x41,
};

static const unsigned char Cp437_EbcdicToAsciiTable [256] =
{
  /* EBCDIC -> Codepage 437/850 */
/*       0     1     2     3     4     5     6     7         8     9     A     B     C     D     E      F*/
/* 0*/ 0x00, 0x01, 0x02, 0x03, 0xBC, 0x09, 0xC5, 0x7F,      0xCD, 0xD9, 0xDA, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
/* 1*/ 0x10, 0x11, 0x12, 0x13, 0xC0, 0xF2, 0x08, 0xCE,      0x18, 0x19, 0xB2, 0xDB, 0x1C, 0x1D, 0x1E, 0x1F,
/* 2*/ 0xD5, 0xC1, 0xC2, 0x9F, 0xC3, 0x0A, 0x17, 0x1B,      0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0x05, 0x06, 0x07,
/* 3*/ 0xB0, 0xB1, 0x16, 0xB3, 0xB4, 0xFE, 0xC4, 0x04,      0xDF, 0xB9, 0xBA, 0xBB, 0x14, 0x15, 0xDC, 0x1A,
/* 4*/ 0x20, 0xFF, 0x83, 0x7B, 0x85, 0xA0, 0xC6, 0x86,      0x87, 0xA4, 0x8E, 0x2E, 0x3C, 0x28, 0x2B, 0x21,
/* 5*/ 0x26, 0x82, 0x88, 0x89, 0x8A, 0xA1, 0x8C, 0x8B,      0x8D, 0xE1, 0x9A, 0x24, 0x2A, 0x29, 0x3B, 0x5E,
/* 6*/ 0x2D, 0x2F, 0xB6, 0x5B, 0xB7, 0xB5, 0xC7, 0x8F,      0x80, 0xA5, 0x94, 0x2C, 0x25, 0x5F, 0x3E, 0x3F,
/* 7*/ 0x9B, 0x90, 0xD2, 0xD3, 0xD4, 0xD6, 0xD7, 0xD8,      0xDE, 0x60, 0x3A, 0x23, 0x40, 0x27, 0x3D, 0x22,
/* 8*/ 0x9D, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,      0x68, 0x69, 0xAE, 0xAF, 0xD0, 0xEC, 0xE7, 0xF1,
/* 9*/ 0xF8, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,      0x71, 0x72, 0xA6, 0xA7, 0x91, 0xF7, 0x92, 0xCF,
/* A*/ 0xE6, 0x7E, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,      0x79, 0x7A, 0xAD, 0xA8, 0xD1, 0xED, 0xE8, 0xA9,
/* B*/ 0xBD, 0x9C, 0xBE, 0xFA, 0xB8, 0xF5, 0xF4, 0xAC,      0xAB, 0xF3, 0xAA, 0x7C, 0xEE, 0xF9, 0xEF, 0x9E,
/* C*/ 0x84, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,      0x48, 0x49, 0xF0, 0x93, 0xDD, 0x95, 0xA2, 0xE4,
/* D*/ 0x81, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,      0x51, 0x52, 0xFB, 0x96, 0x7D, 0x97, 0xA3, 0x98,
/* E*/ 0x99, 0xF6, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,      0x59, 0x5A, 0xFD, 0xE2, 0x5C, 0xE3, 0xE0, 0xE5,
/* F*/ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,      0x38, 0x39, 0xFC, 0xEA, 0x5D, 0xEB, 0xE9, 0xBF,
};

#endif
