#ifndef ICON_BW_PNG_H
#define ICON_BW_PNG_H

#include <wx/mstream.h>
#include <wx/image.h>
#include <wx/bitmap.h>

static const unsigned char icon_bw_png[] = 
{
	0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 
	0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x80, 
	0x00, 0x00, 0x00, 0x80, 0x08, 0x04, 0x00, 0x00, 0x00, 0x69, 
	0x37, 0xA9, 0x40, 0x00, 0x00, 0x00, 0x04, 0x67, 0x41, 0x4D, 
	0x41, 0x00, 0x00, 0xB1, 0x8F, 0x0B, 0xFC, 0x61, 0x05, 0x00, 
	0x00, 0x00, 0x01, 0x73, 0x52, 0x47, 0x42, 0x00, 0xAE, 0xCE, 
	0x1C, 0xE9, 0x00, 0x00, 0x00, 0x20, 0x63, 0x48, 0x52, 0x4D, 
	0x00, 0x00, 0x7A, 0x26, 0x00, 0x00, 0x80, 0x84, 0x00, 0x00, 
	0xFA, 0x00, 0x00, 0x00, 0x80, 0xE8, 0x00, 0x00, 0x75, 0x30, 
	0x00, 0x00, 0xEA, 0x60, 0x00, 0x00, 0x3A, 0x98, 0x00, 0x00, 
	0x17, 0x70, 0x9C, 0xBA, 0x51, 0x3C, 0x00, 0x00, 0x00, 0x02, 
	0x62, 0x4B, 0x47, 0x44, 0x00, 0xFF, 0x87, 0x8F, 0xCC, 0xBF, 
	0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00, 0x00, 
	0x5C, 0x46, 0x00, 0x00, 0x5C, 0x46, 0x01, 0x14, 0x94, 0x43, 
	0x41, 0x00, 0x00, 0x0E, 0xBD, 0x49, 0x44, 0x41, 0x54, 0x78, 
	0xDA, 0xED, 0x5D, 0x79, 0x9C, 0x54, 0xD5, 0x95, 0xFE, 0xCE, 
	0xAB, 0x73, 0xAA, 0x4B, 0xE8, 0x06, 0x1A, 0x85, 0x66, 0xD1, 
	0x88, 0x6C, 0x22, 0x3B, 0x11, 0x85, 0x86, 0x11, 0x62, 0x8C, 
	0xA8, 0x80, 0x10, 0x4D, 0x66, 0x30, 0x1A, 0x19, 0xD4, 0x18, 
	0xD4, 0x19, 0x25, 0xA3, 0x33, 0x93, 0x9F, 0x4B, 0x4C, 0x8C, 
	0x1A, 0xC9, 0xA0, 0x12, 0x07, 0x17, 0xD0, 0x44, 0x45, 0xC7, 
	0xB0, 0xA8, 0x21, 0x20, 0x1A, 0xA2, 0xC3, 0xE2, 0x86, 0x62, 
	0x80, 0x38, 0xC8, 0xA6, 0x2C, 0x01, 0x45, 0x0C, 0x88, 0xD0, 
	0xD2, 0x4D, 0x6F, 0xEF, 0x55, 0x9D, 0xF9, 0xA3, 0x17, 0xBA, 
	0xEA, 0xBD, 0x57, 0x6F, 0xA9, 0x7A, 0x0D, 0x26, 0xFD, 0xF5, 
	0x3F, 0x5D, 0xAF, 0xDE, 0x3D, 0xF7, 0x7C, 0xE7, 0xDE, 0xF7, 
	0xEE, 0xBD, 0xE7, 0x9E, 0x73, 0x0B, 0x68, 0x41, 0x0B, 0x5A, 
	0xD0, 0x82, 0x16, 0xB4, 0xA0, 0x05, 0x2D, 0xF8, 0x3B, 0x05, 
	0x45, 0x26, 0x39, 0xC6, 0xDD, 0xD1, 0x97, 0x7A, 0xA1, 0x9B, 
	0x76, 0xA5, 0x4E, 0xE8, 0xA0, 0xC5, 0xD4, 0xBA, 0xFE, 0x9B, 
	0xC3, 0x28, 0xD3, 0x7D, 0xB4, 0x0F, 0x7B, 0xF4, 0x2F, 0xB4, 
	0x8D, 0x36, 0xD5, 0x7E, 0x82, 0xD4, 0xDF, 0x90, 0x01, 0x62, 
	0xED, 0x8C, 0x91, 0x18, 0xA5, 0x23, 0x30, 0x98, 0x0A, 0x7D, 
	0x16, 0x39, 0xA4, 0xEB, 0x68, 0x35, 0x5E, 0x37, 0xD6, 0xD4, 
	0x1C, 0xF9, 0x4A, 0x1B, 0x40, 0x4E, 0xC3, 0x25, 0x3A, 0x11, 
	0x23, 0x29, 0x16, 0x52, 0x40, 0x8D, 0x2E, 0xA7, 0x25, 0xA9, 
	0xC5, 0xC9, 0xBF, 0x7E, 0xE5, 0x0C, 0x10, 0x2B, 0xA6, 0xCB, 
	0x68, 0x32, 0x86, 0xE5, 0x43, 0x9A, 0x5A, 0xB4, 0x52, 0xE7, 
	0xF2, 0xA2, 0xEA, 0xCA, 0xAF, 0x88, 0x01, 0x64, 0x80, 0xDE, 
	0x44, 0x57, 0xE0, 0x84, 0x3C, 0xEB, 0x55, 0xA6, 0xBF, 0xA6, 
	0x47, 0xCC, 0x5D, 0xC7, 0xB9, 0x01, 0xE4, 0x1F, 0xF4, 0x56, 
	0xBA, 0x28, 0xAA, 0x17, 0xA9, 0x5A, 0xB4, 0x40, 0xA7, 0x5B, 
	0x1B, 0xA3, 0xA4, 0x5F, 0xC4, 0xA1, 0x95, 0x97, 0xC1, 0xB8, 
	0x0F, 0x17, 0x46, 0xA9, 0x1C, 0x00, 0x20, 0x85, 0xDF, 0xE2, 
	0x4E, 0xF3, 0x2F, 0x91, 0xC8, 0x26, 0xBE, 0x82, 0xBE, 0x1B, 
	0xCA, 0x00, 0x5C, 0x42, 0xF7, 0xE2, 0x2A, 0x18, 0x91, 0xD3, 
	0xAF, 0x43, 0x0D, 0x66, 0xE2, 0x17, 0x66, 0x79, 0x7E, 0x85, 
	0x4A, 0x57, 0x3C, 0x82, 0x89, 0x5A, 0x11, 0xDC, 0x00, 0x86, 
	0x5C, 0x83, 0xFF, 0x42, 0xBB, 0x66, 0x22, 0xDF, 0x80, 0x3D, 
	0xFA, 0x2F, 0xD6, 0x92, 0xBC, 0x49, 0x33, 0x78, 0x0A, 0x3D, 
	0x80, 0x76, 0x40, 0x60, 0x03, 0x48, 0x37, 0x3C, 0x89, 0x73, 
	0x03, 0x56, 0x57, 0x85, 0x03, 0xD8, 0xAF, 0x87, 0xA8, 0x12, 
	0xB5, 0x00, 0x12, 0x68, 0xAD, 0xC5, 0xE8, 0x48, 0x27, 0xA2, 
	0x20, 0x98, 0x18, 0x9D, 0x4F, 0xFF, 0x6A, 0x7E, 0x91, 0x3B, 
	0x7B, 0xE9, 0x8D, 0xD9, 0x0D, 0x1C, 0xF4, 0x99, 0x40, 0x06, 
	0x90, 0xEF, 0x61, 0x36, 0xDA, 0xF8, 0xD3, 0x16, 0x7B, 0xF0, 
	0x8E, 0xAE, 0xC3, 0x06, 0x6C, 0xB4, 0x3E, 0x43, 0xD2, 0x7E, 
	0x43, 0x11, 0x57, 0x9D, 0x8A, 0x7E, 0x18, 0x44, 0x67, 0xA2, 
	0x14, 0x1D, 0xFC, 0xBD, 0x48, 0x75, 0x97, 0x31, 0xA0, 0xB6, 
	0x22, 0x17, 0xF2, 0xB1, 0x04, 0xFD, 0x27, 0x6E, 0xA7, 0x78, 
	0xFD, 0xC7, 0x4A, 0x0C, 0x08, 0x50, 0x94, 0xE7, 0x88, 0xFA, 
	0xF8, 0xAB, 0xE4, 0x97, 0xF9, 0x7A, 0x39, 0x2D, 0x80, 0x56, 
	0x06, 0xF7, 0x95, 0x5B, 0x64, 0x15, 0xD7, 0x7A, 0x48, 0x4E, 
	0xF2, 0x15, 0xB9, 0x90, 0x07, 0x64, 0x9C, 0x6C, 0x4B, 0x93, 
	0x78, 0x93, 0xFF, 0xA2, 0x9D, 0xF9, 0x5D, 0x4F, 0xEA, 0x49, 
	0x7E, 0x9D, 0xAF, 0x96, 0xA2, 0xB0, 0xEA, 0x71, 0x07, 0xB9, 
	0x89, 0xFF, 0x2C, 0x29, 0x57, 0xF9, 0xB7, 0xE5, 0x42, 0x9E, 
	0xFB, 0xF2, 0x2B, 0xE9, 0xF2, 0xF8, 0x69, 0xFF, 0xF4, 0x07, 
	0xCA, 0xC7, 0x1E, 0xE4, 0x8F, 0xF0, 0x6C, 0xEE, 0x9B, 0x5B, 
	0xFB, 0x00, 0x00, 0x48, 0x86, 0xF1, 0x7C, 0xC7, 0xBE, 0xB0, 
	0x30, 0xFC, 0x7C, 0x43, 0x4E, 0x97, 0x27, 0xC5, 0xCA, 0x90, 
	0xF7, 0x52, 0x42, 0x00, 0x5F, 0x13, 0x21, 0x1E, 0x4D, 0x8B, 
	0xD1, 0x36, 0xCB, 0x0D, 0x95, 0xFA, 0x28, 0x66, 0x58, 0xFB, 
	0xF3, 0x40, 0xBF, 0x41, 0xE1, 0xEE, 0xB8, 0x43, 0xAF, 0x24, 
	0x6E, 0x72, 0x69, 0x3B, 0xBE, 0xEE, 0x6F, 0x20, 0x94, 0x2E, 
	0x3A, 0x02, 0x5D, 0xA9, 0xBD, 0xB6, 0xA7, 0x76, 0x68, 0x87, 
	0x42, 0x2D, 0x44, 0x1B, 0xEA, 0x65, 0xE3, 0xB9, 0x34, 0xF5, 
	0x8F, 0xC9, 0x6A, 0x7F, 0xCA, 0x8C, 0x95, 0xCA, 0x2C, 0x2D, 
	0x6F, 0xF1, 0xD3, 0xD2, 0x35, 0x7F, 0xD4, 0x9B, 0xD4, 0xDB, 
	0x87, 0x97, 0x36, 0x76, 0x56, 0x93, 0x87, 0x7B, 0x97, 0x88, 
	0x25, 0xE4, 0x5A, 0x7E, 0x4F, 0x92, 0x3E, 0xDE, 0x53, 0xCF, 
	0xD6, 0xB5, 0xBE, 0x2F, 0xFA, 0x5C, 0xE3, 0x2E, 0x88, 0xFF, 
	0xCF, 0x8F, 0x62, 0xE1, 0xC1, 0xE3, 0x65, 0x97, 0xA8, 0xA8, 
	0xFC, 0xD2, 0xC7, 0xBD, 0xA5, 0xBC, 0xD3, 0xD7, 0x4B, 0x3A, 
	0xC9, 0x77, 0xFB, 0x7E, 0x98, 0xF8, 0x1B, 0x52, 0xE5, 0x4A, 
	0xBE, 0x56, 0xEE, 0xF2, 0x6D, 0xC7, 0xD0, 0x90, 0x22, 0x9E, 
	0x23, 0xDB, 0x62, 0x09, 0x4F, 0x4D, 0x87, 0xC8, 0x11, 0x3F, 
	0xF4, 0xB9, 0x42, 0xBE, 0xE7, 0xBF, 0xF2, 0x41, 0xF2, 0xA5, 
	0xAB, 0xA0, 0x9D, 0xD1, 0xB6, 0x7D, 0x53, 0xC4, 0xDA, 0x79, 
	0xDF, 0xC3, 0xAF, 0xF9, 0x6A, 0xFD, 0x8F, 0x64, 0x60, 0x66, 
	0x49, 0xD7, 0xCE, 0x20, 0x5D, 0x75, 0x0D, 0xB9, 0x3D, 0xDD, 
	0xCB, 0x52, 0x97, 0x27, 0x0F, 0x35, 0x97, 0x01, 0xBC, 0x91, 
	0x90, 0x64, 0x19, 0x5A, 0xB9, 0x7E, 0xFD, 0xA5, 0x2E, 0xA3, 
	0x8D, 0xD8, 0xA4, 0xFB, 0x52, 0xEF, 0xA7, 0xFC, 0x7A, 0x19, 
	0x62, 0x09, 0x7E, 0xCF, 0xC5, 0x8A, 0x29, 0x99, 0xD9, 0x6C, 
	0xCB, 0x20, 0x9F, 0x28, 0x89, 0xC9, 0x21, 0xB7, 0x2E, 0xCF, 
	0x0F, 0xC8, 0x89, 0x21, 0x44, 0xCA, 0x6F, 0x5C, 0x5F, 0x21, 
	0xFF, 0x76, 0xAC, 0xE9, 0x3A, 0x81, 0xE7, 0x3B, 0xE8, 0xBA, 
	0x4C, 0x6E, 0x8C, 0xB7, 0x0F, 0x25, 0x4E, 0x26, 0xBB, 0x0E, 
	0x7A, 0x57, 0x1D, 0x6B, 0xAA, 0x2E, 0x06, 0xE8, 0x6B, 0x1B, 
	0xAD, 0x36, 0x86, 0x9E, 0x38, 0x49, 0x77, 0x3E, 0xEC, 0xD2, 
	0xFA, 0xC7, 0x29, 0x7D, 0x00, 0x90, 0x1B, 0x33, 0x3A, 0xFF, 
	0x0D, 0x61, 0x25, 0xC5, 0xF8, 0x2D, 0xE7, 0x67, 0x9F, 0xA7, 
	0x1D, 0x6B, 0x92, 0xD9, 0xC0, 0x13, 0xD3, 0xB4, 0x2D, 0x8B, 
	0xFB, 0x74, 0xCA, 0xDB, 0x5E, 0x67, 0x72, 0x1D, 0x8D, 0x74, 
	0xBA, 0x51, 0x1F, 0xB4, 0x1E, 0x3A, 0xD6, 0x24, 0xB3, 0xE2, 
	0x8C, 0x34, 0x6D, 0x9F, 0xCD, 0xB2, 0x6C, 0x8E, 0xC9, 0xAD, 
	0x70, 0x73, 0xDD, 0x4B, 0x17, 0xE7, 0xB1, 0x9F, 0x97, 0x1E, 
	0x6F, 0x6F, 0x7E, 0x9B, 0xE6, 0xCB, 0x9A, 0xBE, 0xAB, 0xA4, 
	0xB7, 0xDB, 0x7D, 0xDC, 0x46, 0x16, 0x8B, 0xCA, 0x30, 0x37, 
	0x31, 0xFF, 0xE3, 0xD8, 0xFD, 0xB7, 0x73, 0xB6, 0xC5, 0xD0, 
	0x71, 0x00, 0x39, 0x31, 0x6D, 0x05, 0xF9, 0xAA, 0xEB, 0x7D, 
	0xDD, 0x78, 0x83, 0xA8, 0xA8, 0xFC, 0xCC, 0xF9, 0xEB, 0x61, 
	0x4E, 0xAB, 0x71, 0xAE, 0x91, 0xA1, 0x91, 0x33, 0x30, 0x64, 
	0x90, 0x4C, 0x95, 0xA9, 0xDC, 0x3F, 0xA4, 0x01, 0x6E, 0x4E, 
	0xD3, 0xF9, 0xAF, 0xF2, 0x08, 0x8F, 0x61, 0x9B, 0xCB, 0x4D, 
	0xCE, 0x91, 0xFD, 0xF5, 0x77, 0xBC, 0xE3, 0x2C, 0x66, 0x85, 
	0x63, 0xFB, 0xDF, 0x1E, 0x25, 0xF3, 0x58, 0x82, 0xC7, 0xF0, 
	0xAC, 0x46, 0x7F, 0x43, 0x4A, 0x16, 0x06, 0x77, 0xA9, 0x18, 
	0xAD, 0x78, 0x8F, 0x83, 0xDE, 0x6B, 0x8D, 0xA6, 0xB3, 0x43, 
	0x92, 0x6B, 0x9B, 0x0C, 0x95, 0x56, 0x41, 0xB1, 0x9D, 0xFE, 
	0x79, 0x8E, 0xF4, 0xD7, 0x22, 0xEC, 0x5E, 0x9F, 0x07, 0xB8, 
	0xA3, 0x5C, 0x29, 0xCF, 0x73, 0xB9, 0xAD, 0xC7, 0x2D, 0x0F, 
	0x5A, 0xA3, 0xDC, 0xE6, 0x32, 0x0F, 0x5C, 0x53, 0xEF, 0xA4, 
	0x21, 0x19, 0x20, 0x4B, 0x32, 0xBE, 0x1B, 0x6D, 0x17, 0xB3, 
	0xCA, 0x71, 0xEA, 0x73, 0x66, 0xFE, 0xA9, 0xCB, 0x19, 0xF2, 
	0x1F, 0xF2, 0x36, 0x9B, 0xAE, 0x8B, 0x96, 0x1F, 0x04, 0x92, 
	0xD6, 0xDB, 0x7D, 0xCD, 0x2A, 0x49, 0x59, 0xCF, 0x4B, 0x79, 
	0xAB, 0xED, 0xD1, 0xFE, 0xBD, 0x6D, 0x9A, 0x24, 0x67, 0x3B, 
	0x3E, 0xFF, 0x73, 0xF2, 0x49, 0x3C, 0x1E, 0xE7, 0xD1, 0xFC, 
	0x40, 0x86, 0x63, 0xD2, 0xA9, 0xE5, 0xDE, 0xF5, 0x2F, 0x33, 
	0x96, 0x90, 0xB5, 0x5E, 0xF2, 0x32, 0x1B, 0x55, 0x1E, 0x3E, 
	0xBA, 0x90, 0x6F, 0xB4, 0x83, 0x3C, 0x87, 0xCB, 0x33, 0x85, 
	0x6B, 0x39, 0x7A, 0xE6, 0xC7, 0xD5, 0x15, 0x2B, 0x36, 0xC6, 
	0x60, 0x02, 0xC6, 0xFA, 0xDB, 0x50, 0xD1, 0x5A, 0xAB, 0x95, 
	0x93, 0x2B, 0x3D, 0x03, 0x06, 0x0F, 0xA1, 0x52, 0x8C, 0x0B, 
	0xB4, 0x41, 0x57, 0xA5, 0xBF, 0xC7, 0xFD, 0xD6, 0xFA, 0xA3, 
	0x17, 0xEA, 0xFD, 0x6E, 0xDC, 0x41, 0x9D, 0x76, 0xC9, 0x1E, 
	0xCA, 0x99, 0x3E, 0x71, 0x77, 0x1A, 0xA7, 0x13, 0x30, 0x1A, 
	0x1C, 0xA0, 0x50, 0x2C, 0x1E, 0xAB, 0xCD, 0x6A, 0x80, 0x22, 
	0xAE, 0xBE, 0x0A, 0x3F, 0x46, 0x0F, 0x0F, 0x43, 0x5A, 0xA8, 
	0x45, 0x12, 0x35, 0xA8, 0xC0, 0x01, 0xDA, 0x8D, 0x4D, 0x78, 
	0x2F, 0xB9, 0x32, 0x73, 0x41, 0x5C, 0xAF, 0x16, 0xFD, 0x33, 
	0xE2, 0xB6, 0xE2, 0xE5, 0xFA, 0x60, 0x0E, 0xD4, 0x63, 0x7C, 
	0x36, 0x2E, 0xC6, 0xB7, 0xA9, 0x0F, 0x28, 0xE8, 0xAA, 0x44, 
	0x3F, 0x36, 0x6B, 0xB3, 0x7D, 0xCF, 0x27, 0x57, 0x2F, 0x44, 
	0xA9, 0x6B, 0xE9, 0xE5, 0xB4, 0x44, 0xF7, 0xD2, 0x5E, 0xDD, 
	0x63, 0x1C, 0x2C, 0xA8, 0x2E, 0xB7, 0xB2, 0xD7, 0x55, 0x67, 
	0x00, 0xC2, 0x14, 0xFB, 0x57, 0xF4, 0x6B, 0x2B, 0x94, 0xD3, 
	0x23, 0x21, 0xD6, 0x58, 0x9A, 0x88, 0xF1, 0xE8, 0x10, 0xDA, 
	0x78, 0xF3, 0xB2, 0xAA, 0xDC, 0x0B, 0x2B, 0x91, 0xC5, 0x11, 
	0x4B, 0x33, 0xCD, 0x97, 0x1B, 0xFE, 0xCF, 0x6A, 0xC7, 0x7A, 
	0xEA, 0x00, 0x78, 0x00, 0x6D, 0xB0, 0xD9, 0xD1, 0xA2, 0x5E, 
	0x61, 0x02, 0x14, 0xF8, 0x14, 0x2C, 0xA5, 0x81, 0xC1, 0xCB, 
	0x35, 0xC1, 0x46, 0x63, 0xB8, 0x7B, 0xB4, 0x50, 0x41, 0x71, 
	0x72, 0x2D, 0x75, 0xCF, 0x52, 0xBA, 0x86, 0x4E, 0x0A, 0xB2, 
	0x7D, 0x56, 0xD7, 0x03, 0x2E, 0x71, 0xB0, 0xCC, 0xFF, 0x86, 
	0xA1, 0x5F, 0xC4, 0xD5, 0xBF, 0x43, 0x2E, 0xF4, 0xCB, 0x74, 
	0xA1, 0x71, 0x6B, 0xCD, 0x11, 0x2E, 0xA0, 0x01, 0x7A, 0x16, 
	0xF5, 0xD3, 0xD3, 0xA8, 0x13, 0x8A, 0x60, 0xE0, 0x30, 0xF6, 
	0xE9, 0x76, 0xAC, 0xA7, 0x55, 0xC9, 0xE9, 0x59, 0xE9, 0x43, 
	0xDF, 0x32, 0x03, 0xED, 0x1E, 0x32, 0x00, 0xD0, 0x04, 0x07, 
	0x41, 0x4F, 0x85, 0xD1, 0xBF, 0xFA, 0x7C, 0x84, 0x9C, 0x36, 
	0xEB, 0x2E, 0x2C, 0xC5, 0x12, 0xE3, 0x75, 0x4D, 0xA4, 0x26, 
	0xF2, 0x25, 0x38, 0x1F, 0x85, 0x84, 0x74, 0x97, 0x25, 0x01, 
	0x80, 0x7A, 0xBD, 0x50, 0x68, 0x41, 0xB0, 0x7A, 0x19, 0xE0, 
	0x12, 0x7C, 0xDD, 0x76, 0xFD, 0xB0, 0xBE, 0x14, 0x8A, 0xC6, 
	0xB9, 0x81, 0x5F, 0x78, 0x16, 0xD6, 0xD3, 0x4B, 0x58, 0x62, 
	0x6D, 0x00, 0xC9, 0xD0, 0xD4, 0x1C, 0x9A, 0x94, 0x35, 0xDE, 
	0xC8, 0x43, 0xBC, 0x96, 0xD3, 0xFC, 0xC0, 0x06, 0xA0, 0x73, 
	0xED, 0x62, 0x75, 0x59, 0xB2, 0x2A, 0x98, 0x20, 0x39, 0x51, 
	0xAF, 0xA1, 0x6F, 0x23, 0xC0, 0xAA, 0x51, 0x8F, 0xD0, 0x0A, 
	0xBC, 0x84, 0xC5, 0x75, 0x43, 0x2D, 0x8F, 0xA6, 0xBB, 0x30, 
	0x2A, 0xF0, 0x80, 0x91, 0x69, 0x9F, 0x27, 0x83, 0x46, 0x92, 
	0x30, 0x80, 0x51, 0x0E, 0xD7, 0x83, 0xB5, 0xBF, 0x21, 0xDD, 
	0x74, 0x01, 0x0D, 0x05, 0xB0, 0xC9, 0x37, 0xFD, 0x83, 0xB4, 
	0x02, 0xD5, 0x18, 0x45, 0xA3, 0x04, 0x80, 0x76, 0xA1, 0x6F, 
	0xE6, 0x46, 0x1D, 0x00, 0xB4, 0x82, 0x7C, 0xEC, 0x21, 0xD9, 
	0x0D, 0x70, 0x8E, 0xED, 0x6A, 0x92, 0x96, 0x05, 0x60, 0xDF, 
	0x2A, 0x36, 0x46, 0x8B, 0xA9, 0xEE, 0xD9, 0x4F, 0x41, 0xFD, 
	0x39, 0x23, 0xA9, 0x3D, 0xBE, 0xDB, 0xE4, 0x53, 0xEE, 0xEC, 
	0x01, 0xD0, 0x74, 0xF3, 0xB3, 0xA0, 0x65, 0x8C, 0x78, 0xA1, 
	0xF6, 0xB1, 0x5D, 0xFD, 0xD0, 0x3C, 0xE0, 0x5F, 0x44, 0x6C, 
	0x94, 0x0E, 0xA0, 0xFD, 0xD8, 0x0C, 0x00, 0xE8, 0xA5, 0x5B, 
	0xF3, 0xC2, 0x25, 0x04, 0x74, 0x4D, 0x22, 0x70, 0xFB, 0x03, 
	0x9C, 0x3A, 0x9D, 0xEC, 0x93, 0xD4, 0xD5, 0xFE, 0x05, 0xC4, 
	0xDB, 0xA7, 0xBA, 0xD2, 0x3E, 0xF3, 0x65, 0xD9, 0x89, 0x99, 
	0x50, 0xBC, 0x8C, 0x55, 0x98, 0x87, 0x90, 0x6E, 0x8D, 0x9C, 
	0xE8, 0xEF, 0xA6, 0x4B, 0xBD, 0x66, 0x7D, 0x8E, 0x06, 0xA0, 
	0x7E, 0x0E, 0xC2, 0xD6, 0xF9, 0x17, 0x90, 0xEA, 0x82, 0xAE, 
	0xFA, 0x02, 0x60, 0x6E, 0x69, 0x58, 0x94, 0xC4, 0x4B, 0x53, 
	0xB3, 0x68, 0x4A, 0x33, 0xF3, 0xDF, 0x4C, 0xE3, 0xCD, 0xBD, 
	0x61, 0x0A, 0x1A, 0xDA, 0xCB, 0xE1, 0xEA, 0xFB, 0x41, 0x44, 
	0x90, 0x91, 0x1E, 0x1E, 0x9D, 0x1A, 0x88, 0x09, 0x41, 0xCA, 
	0xE7, 0x0A, 0xB5, 0xF4, 0xE1, 0xE4, 0x59, 0x61, 0x83, 0x29, 
	0x99, 0x4E, 0xB5, 0x4B, 0x34, 0x3E, 0x0A, 0x20, 0xE1, 0x73, 
	0xDD, 0x4E, 0x4D, 0x3C, 0xC6, 0xDC, 0x17, 0xCB, 0x28, 0x74, 
	0x9C, 0x50, 0x60, 0x54, 0xEB, 0xF3, 0x74, 0x9F, 0xB9, 0x25, 
	0xBC, 0x00, 0xC6, 0x29, 0xB6, 0x6B, 0x5F, 0xD6, 0x1E, 0xF4, 
	0x2F, 0xC0, 0xFA, 0x5C, 0x2A, 0x71, 0x32, 0x36, 0x40, 0x01, 
	0x00, 0x31, 0x7A, 0x06, 0xCD, 0x43, 0x3F, 0xA9, 0xEF, 0xD0, 
	0xF3, 0x78, 0xCE, 0xCA, 0x31, 0x72, 0x90, 0xB5, 0xC4, 0x36, 
	0x04, 0xED, 0x09, 0x24, 0x21, 0x85, 0xCD, 0x3A, 0x92, 0x7B, 
	0x58, 0xDB, 0x01, 0x80, 0xC7, 0x23, 0x02, 0x17, 0x5A, 0x53, 
	0xE8, 0x11, 0xDA, 0xA2, 0xEF, 0xE2, 0x0D, 0x5A, 0x91, 0x2B, 
	0xF5, 0x7A, 0x03, 0x90, 0x6D, 0xFF, 0x54, 0x03, 0x3A, 0x41, 
	0xCC, 0x0F, 0xF9, 0x2C, 0xF4, 0x8C, 0x1F, 0xAC, 0x3D, 0x08, 
	0x60, 0x72, 0x64, 0xCC, 0x77, 0xE8, 0x5C, 0x6C, 0xA5, 0x0F, 
	0x78, 0x47, 0xB5, 0x99, 0x4F, 0xB1, 0x6C, 0x8F, 0xFC, 0x24, 
	0x9F, 0x0F, 0x00, 0xB7, 0xA1, 0xC9, 0xB8, 0x00, 0xDD, 0x60, 
	0x62, 0x87, 0xC6, 0x52, 0x85, 0x78, 0x11, 0x06, 0x7D, 0x2B, 
	0x02, 0xEA, 0x29, 0xBC, 0xA9, 0xB3, 0xAC, 0x45, 0x75, 0x99, 
	0x45, 0x79, 0x65, 0x0F, 0x80, 0xED, 0x4B, 0x0F, 0xF5, 0x15, 
	0x45, 0xC1, 0x17, 0xD0, 0x33, 0xE8, 0x58, 0xFF, 0x61, 0x08, 
	0x41, 0xC7, 0xF1, 0x14, 0xDC, 0xEF, 0x33, 0x90, 0x36, 0x08, 
	0x5E, 0xC5, 0x34, 0x33, 0xC2, 0xC9, 0x95, 0x93, 0xA7, 0xAE, 
	0xC6, 0xBB, 0x98, 0x5C, 0xAA, 0x0B, 0xD2, 0xCB, 0x52, 0x1C, 
	0xE3, 0x74, 0x4C, 0x7E, 0x95, 0xD3, 0x72, 0xFC, 0xD8, 0x7A, 
	0x2C, 0x3A, 0xF2, 0x6E, 0x06, 0xF0, 0xA6, 0xDF, 0x1B, 0xCF, 
	0x3A, 0xCC, 0x1F, 0x41, 0x79, 0x8D, 0x1A, 0x53, 0x0B, 0x23, 
	0xAD, 0x0F, 0xA2, 0xA5, 0x0F, 0xC7, 0x3D, 0x5F, 0xEF, 0x30, 
	0xF6, 0xBB, 0xB3, 0x84, 0x24, 0xE5, 0x0D, 0xC4, 0x4E, 0x9E, 
	0xCA, 0xFC, 0x1B, 0xC0, 0xF6, 0xC4, 0x93, 0x07, 0x39, 0x29, 
	0xC2, 0xC4, 0xE8, 0x15, 0x03, 0x00, 0x9A, 0x16, 0xFD, 0xB6, 
	0xAC, 0x01, 0x9B, 0x03, 0x41, 0x3D, 0x02, 0x8B, 0x74, 0x78, 
	0xD0, 0x54, 0x87, 0xD0, 0x88, 0xE1, 0xF1, 0xA2, 0x50, 0x0F, 
	0x69, 0x00, 0x03, 0xA8, 0x6D, 0xD0, 0xA3, 0x8E, 0x1E, 0x65, 
	0x9A, 0x73, 0xAD, 0x37, 0xA4, 0xEA, 0xDF, 0x23, 0x36, 0x00, 
	0xD9, 0xF3, 0x34, 0x4F, 0xF6, 0x28, 0xD3, 0xB9, 0x19, 0x0D, 
	0x00, 0xFA, 0x09, 0xF7, 0x8C, 0xD4, 0x00, 0x0E, 0x13, 0xDF, 
	0xB6, 0x1E, 0xD1, 0x75, 0xD1, 0x3D, 0x00, 0xBB, 0xF5, 0xA7, 
	0x58, 0x9B, 0x71, 0xAD, 0x15, 0x66, 0x47, 0x98, 0xE2, 0x0D, 
	0x03, 0xBB, 0x6C, 0xD7, 0x28, 0x75, 0x7A, 0xB6, 0x22, 0x54, 
	0x16, 0x85, 0x22, 0xFA, 0x27, 0x5C, 0x9E, 0xE8, 0x69, 0xFD, 
	0xDC, 0x1C, 0x86, 0xA9, 0x9A, 0xB6, 0x23, 0x45, 0xE7, 0xF1, 
	0x94, 0x28, 0x0D, 0xB0, 0xCD, 0xE1, 0xEA, 0xA0, 0xAC, 0xAA, 
	0x6E, 0x43, 0x7E, 0x91, 0xC2, 0x52, 0x8C, 0xB6, 0x86, 0x99, 
	0xF3, 0xCA, 0x2D, 0x00, 0x29, 0xF3, 0x71, 0xEA, 0xA7, 0xAF, 
	0xA4, 0xDD, 0x71, 0x3F, 0x7B, 0xBD, 0x97, 0xC2, 0x1B, 0x40, 
	0x37, 0xDB, 0x2F, 0x52, 0xD6, 0xC1, 0x87, 0xDE, 0xAA, 0x5F, 
	0xFA, 0x7A, 0xC1, 0xCF, 0x5D, 0x95, 0xFA, 0x04, 0xFA, 0x99, 
	0x17, 0x9B, 0x6F, 0x34, 0xBD, 0xDB, 0xFC, 0xCC, 0x1A, 0xAF, 
	0xD7, 0x6B, 0xE3, 0xF6, 0x18, 0xB5, 0xC7, 0x7F, 0x07, 0xE5, 
	0xC5, 0x3E, 0x27, 0xE5, 0x14, 0x2F, 0x4C, 0x1D, 0xB2, 0xCD, 
	0xEA, 0x36, 0x9B, 0xFD, 0xB2, 0x15, 0xE2, 0x37, 0xE8, 0x1C, 
	0x64, 0x47, 0x19, 0x26, 0xA0, 0x1A, 0x4F, 0x23, 0x5B, 0x1E, 
	0xD1, 0xE7, 0x78, 0x0C, 0xB3, 0xDC, 0xDD, 0xAF, 0x72, 0x86, 
	0xCE, 0x6F, 0xDC, 0x65, 0x54, 0x5C, 0x7C, 0x74, 0xCB, 0x33, 
	0x13, 0xF1, 0x78, 0x6A, 0x17, 0x25, 0xB5, 0x92, 0x4C, 0x00, 
	0x50, 0x03, 0x6D, 0xA9, 0x58, 0xF7, 0x59, 0x7E, 0x33, 0xD7, 
	0xEA, 0x03, 0xC7, 0xD2, 0x63, 0x28, 0x4E, 0xCA, 0x56, 0x42, 
	0xCE, 0xCB, 0x92, 0xDB, 0x55, 0x17, 0xE3, 0x71, 0x6F, 0x9D, 
	0x62, 0xF2, 0xA8, 0x63, 0xF8, 0x4A, 0xA5, 0xBC, 0xCD, 0xD7, 
	0xC5, 0x3C, 0x33, 0xCE, 0x63, 0x27, 0xF0, 0xEC, 0x86, 0x9A, 
	0x78, 0x2F, 0xFF, 0x94, 0x5D, 0xA2, 0xFF, 0x78, 0xB8, 0x83, 
	0x06, 0x2F, 0xFA, 0xA4, 0x0F, 0xC8, 0xA3, 0x0E, 0xC5, 0xAF, 
	0xF4, 0x28, 0xF3, 0xF0, 0xD1, 0x20, 0x2A, 0xBE, 0x93, 0xB7, 
	0xD8, 0x24, 0xD4, 0x6F, 0xB7, 0xF2, 0xF9, 0x7C, 0xBD, 0x2C, 
	0x16, 0x95, 0x95, 0xB2, 0x9A, 0x5F, 0x93, 0xE7, 0x64, 0x3A, 
	0x5F, 0x23, 0x67, 0x73, 0x80, 0x71, 0x84, 0xAF, 0xE0, 0x8A, 
	0x46, 0xAD, 0x4C, 0xBE, 0x19, 0x90, 0xEE, 0xFC, 0xB4, 0xFC, 
	0x53, 0xDA, 0x3D, 0x77, 0x3A, 0x18, 0xD9, 0xFF, 0xFC, 0x41, 
	0x26, 0x39, 0x14, 0xF7, 0xD8, 0x62, 0x4C, 0x08, 0xCF, 0xAB, 
	0x0F, 0xA1, 0x8B, 0x01, 0x30, 0x32, 0xE3, 0xCB, 0x1B, 0x0C, 
	0x18, 0xEB, 0x24, 0x93, 0x78, 0x84, 0x7C, 0xCC, 0x33, 0xFC, 
	0x53, 0xB6, 0x99, 0xA0, 0x3F, 0x6F, 0x6D, 0x94, 0x9C, 0xE2, 
	0x7B, 0xB8, 0x5C, 0x94, 0x7F, 0x92, 0xC6, 0xC0, 0x29, 0x4A, 
	0xE8, 0x2C, 0xFF, 0x15, 0x74, 0xB4, 0x77, 0x68, 0x3E, 0xEC, 
	0xD9, 0x41, 0x49, 0x6E, 0x61, 0x53, 0x1A, 0x36, 0xB4, 0x8C, 
	0x8C, 0xD0, 0xA7, 0x43, 0x32, 0x12, 0x00, 0xE4, 0x6C, 0xF9, 
	0x25, 0xF7, 0xE7, 0x31, 0x6C, 0xCA, 0x79, 0x39, 0x98, 0xA0, 
	0x0D, 0xBF, 0x99, 0xA1, 0xDF, 0x75, 0x4D, 0xE8, 0x9F, 0xE6, 
	0xA0, 0xFF, 0x17, 0x7E, 0x43, 0xED, 0x0C, 0xC0, 0xDA, 0x0F, 
	0xDB, 0x3E, 0x00, 0x15, 0x19, 0x5E, 0xAE, 0x6D, 0x35, 0x1F, 
	0xC0, 0x7E, 0x94, 0xD4, 0x7F, 0x4A, 0x69, 0xFA, 0xAE, 0x6C, 
	0x3B, 0xCC, 0x00, 0x00, 0xED, 0x86, 0x6A, 0xAA, 0xB0, 0x5E, 
	0xA5, 0x07, 0x31, 0x4F, 0xBA, 0x23, 0x24, 0xAC, 0xC3, 0x18, 
	0x8F, 0x3F, 0xA7, 0xE9, 0xD7, 0x44, 0x63, 0x9D, 0x64, 0x9F, 
	0x28, 0xD1, 0x72, 0x1F, 0x41, 0x56, 0x0D, 0x06, 0x00, 0x1C, 
	0xB7, 0xC2, 0xA7, 0x78, 0x17, 0xA6, 0xDD, 0x18, 0x0A, 0xC0, 
	0x88, 0x17, 0xF2, 0xB9, 0x34, 0x56, 0x3F, 0x4D, 0xB3, 0xCF, 
	0x36, 0x18, 0x3C, 0x1A, 0x83, 0xF5, 0x4F, 0xE6, 0x6E, 0xA0, 
	0xFD, 0x6D, 0xFA, 0x1E, 0xFE, 0x18, 0x3E, 0xC3, 0xD0, 0xFA, 
	0x52, 0x2F, 0xD0, 0x0F, 0x1B, 0x65, 0x6F, 0x31, 0x8F, 0x1A, 
	0xC0, 0xA0, 0xAB, 0xED, 0xF7, 0x6B, 0xB0, 0x54, 0x7B, 0xEE, 
	0xEF, 0xD0, 0x89, 0x4C, 0xEF, 0x04, 0x68, 0x9E, 0xC8, 0x35, 
	0xB2, 0x91, 0x3F, 0x17, 0x8B, 0x2B, 0xF8, 0x4D, 0xE9, 0xDD, 
	0x34, 0xC5, 0x92, 0x9F, 0xE0, 0x1F, 0xC9, 0x7D, 0x3C, 0xAD, 
	0x61, 0x3C, 0x2E, 0x68, 0x2D, 0x2B, 0x79, 0x67, 0x2E, 0xE9, 
	0xB5, 0xFC, 0x35, 0xF9, 0x03, 0xD7, 0x88, 0xF2, 0x0E, 0x1E, 
	0x72, 0xF4, 0xAA, 0x7C, 0xD3, 0x69, 0x94, 0xF1, 0x3B, 0x0B, 
	0x68, 0x00, 0xC9, 0x07, 0x0E, 0x62, 0x66, 0x7A, 0x17, 0x94, 
	0x6B, 0x65, 0x2A, 0x4F, 0xE0, 0x21, 0x75, 0x81, 0x87, 0xFC, 
	0x7E, 0x23, 0xFD, 0xBD, 0x32, 0x85, 0x7F, 0x21, 0x63, 0x9B, 
	0x3A, 0x5C, 0x62, 0x09, 0x59, 0xC8, 0xE5, 0xFC, 0xA3, 0x44, 
	0x0E, 0xCE, 0x94, 0xD8, 0x09, 0xD2, 0x25, 0xDD, 0x89, 0x23, 
	0x8B, 0x1D, 0xC6, 0xB0, 0x45, 0x81, 0x05, 0xCB, 0x2D, 0x0E, 
	0x62, 0xCA, 0x83, 0xA6, 0x1C, 0x71, 0x89, 0x3C, 0x28, 0x1F, 
	0xF1, 0x5E, 0xFE, 0x2D, 0xF7, 0x8C, 0x15, 0x3B, 0x0C, 0x76, 
	0x24, 0x3F, 0xE4, 0xCF, 0xA4, 0x4C, 0x9E, 0x93, 0xA9, 0x32, 
	0x92, 0x4F, 0xE1, 0xB6, 0x5C, 0x10, 0x8F, 0xC7, 0x0B, 0xA5, 
	0x33, 0xDF, 0x2B, 0x2B, 0x79, 0x46, 0xF0, 0xFC, 0x2E, 0xEE, 
	0xE7, 0x38, 0x23, 0xB9, 0x24, 0xA8, 0x1C, 0x70, 0x07, 0xA9, 
	0x76, 0x30, 0xC1, 0x3D, 0x81, 0x05, 0x01, 0x1E, 0x9B, 0xFD, 
	0x09, 0xE1, 0xEF, 0xF0, 0x53, 0xBC, 0x85, 0x6B, 0x24, 0x25, 
	0x49, 0xB1, 0xD8, 0x12, 0xAB, 0x71, 0xB2, 0xB3, 0x33, 0x7E, 
	0xAA, 0xDF, 0x4A, 0xEA, 0xF5, 0x9E, 0xE7, 0xA0, 0xF5, 0xDE, 
	0x50, 0x4E, 0x14, 0xA7, 0x54, 0x09, 0x3E, 0xCC, 0x25, 0x21, 
	0x44, 0xF9, 0x42, 0x3C, 0x2E, 0x5D, 0x65, 0x00, 0x8F, 0x90, 
	0x73, 0xE4, 0x4C, 0xF9, 0x21, 0x2F, 0xE2, 0x03, 0xA2, 0xA2, 
	0xFC, 0x5A, 0x20, 0x9D, 0x07, 0x3B, 0xA6, 0x4A, 0xFF, 0x3C, 
	0x94, 0x42, 0x2E, 0xC1, 0xD2, 0x8F, 0x47, 0x65, 0x00, 0x1B, 
	0x0C, 0xEE, 0x21, 0x17, 0x49, 0x90, 0x10, 0x3B, 0xE2, 0xE5, 
	0x0E, 0xF4, 0xAB, 0x62, 0x9D, 0x42, 0x6A, 0x20, 0x2B, 0x1D, 
	0x23, 0xAB, 0xA3, 0xCF, 0x16, 0x09, 0x09, 0xFE, 0x8E, 0xE3, 
	0x3A, 0xE4, 0x89, 0xD0, 0x02, 0x1D, 0x07, 0x14, 0x95, 0x75, 
	0x25, 0x11, 0x25, 0x4C, 0xE4, 0x48, 0xBF, 0x2D, 0x7F, 0xEA, 
	0xD0, 0x63, 0x6B, 0xB9, 0x47, 0x2E, 0x42, 0x97, 0x3B, 0x9A, 
	0xE0, 0x8E, 0x63, 0x4D, 0xD6, 0x09, 0xF2, 0xA4, 0x93, 0xAE, 
	0x1C, 0xBE, 0xFD, 0x01, 0x40, 0x86, 0x39, 0xBD, 0x54, 0xB8, 
	0x36, 0xC0, 0xC2, 0xA2, 0x99, 0xE0, 0xDC, 0xFD, 0xB9, 0x22, 
	0xE7, 0xD3, 0x2C, 0xE4, 0x59, 0x47, 0xC1, 0x3B, 0xFC, 0xE4, 
	0xF0, 0x37, 0x1F, 0xA4, 0xBB, 0x4B, 0xB6, 0xF8, 0xCF, 0x72, 
	0x17, 0xED, 0x96, 0x38, 0xF9, 0x4A, 0x54, 0xA9, 0x53, 0xC1, 
	0x51, 0xD0, 0xFA, 0xE8, 0x8C, 0x33, 0x4D, 0xC7, 0x9D, 0xDE, 
	0x2E, 0x16, 0x1F, 0xE0, 0x1B, 0x1C, 0x6D, 0xAB, 0x92, 0x4B, 
	0xF2, 0x44, 0x3E, 0x11, 0xE3, 0x17, 0x1D, 0xF5, 0x4B, 0x49, 
	0x9E, 0xCE, 0xB6, 0x8B, 0x65, 0xAE, 0xBD, 0x1B, 0x2D, 0x7C, 
	0x3C, 0x9C, 0x1C, 0x40, 0x3C, 0xCB, 0x45, 0xBB, 0xB9, 0x79, 
	0xAB, 0x43, 0xBA, 0xBB, 0x9C, 0x1D, 0x92, 0xE4, 0xAB, 0x73, 
	0x97, 0x9E, 0x23, 0xFD, 0x7B, 0x5C, 0xFA, 0xE7, 0xEE, 0xBC, 
	0x26, 0xF7, 0xF2, 0xF7, 0x5D, 0xAA, 0x39, 0xB6, 0x26, 0x20, 
	0xBE, 0xDB, 0xA5, 0xF5, 0x4D, 0xF1, 0xF2, 0x52, 0x07, 0x85, 
	0x3C, 0xE1, 0x62, 0x82, 0x94, 0xDC, 0x7C, 0x8C, 0xE8, 0xC7, 
	0xF8, 0x21, 0x17, 0x9D, 0x54, 0x6E, 0xCD, 0x7F, 0x6D, 0x09, 
	0x5E, 0xE3, 0x5A, 0xDD, 0xAF, 0xA2, 0xDE, 0xB2, 0xB6, 0xA3, 
	0xA0, 0xB5, 0xBC, 0xE0, 0xAA, 0xCF, 0x0B, 0x91, 0xEC, 0x1D, 
	0x4A, 0x17, 0xF9, 0xC4, 0xB5, 0xCA, 0x65, 0x21, 0x0F, 0x27, 
	0x09, 0xAB, 0x4B, 0x37, 0x59, 0xEF, 0xAA, 0xCB, 0xFA, 0x5C, 
	0x1C, 0x2C, 0xD9, 0xAB, 0x1D, 0x28, 0x65, 0x6E, 0xD5, 0xF2, 
	0x2E, 0x2E, 0xCD, 0xBD, 0x06, 0x7F, 0xE0, 0x09, 0xFC, 0x85, 
	0x2B, 0xFD, 0x5D, 0xD2, 0x25, 0xCA, 0xAA, 0x47, 0xBB, 0x1F, 
	0x50, 0xC5, 0xA6, 0xDC, 0x15, 0x8F, 0xE7, 0x5E, 0x47, 0x76, 
	0x48, 0x11, 0xCF, 0xC9, 0xB2, 0x0B, 0xB5, 0x4F, 0x4E, 0xCF, 
	0xBD, 0x8E, 0xEC, 0x0A, 0x5C, 0x94, 0x25, 0x33, 0x5B, 0x65, 
	0x03, 0x8F, 0x88, 0xB0, 0x72, 0xE2, 0x8B, 0x65, 0xB7, 0x7B, 
	0xED, 0x7C, 0xA0, 0xA9, 0x73, 0x34, 0x32, 0xF0, 0x85, 0x59, 
	0x8F, 0x29, 0x4B, 0xF2, 0x5C, 0xF6, 0x8A, 0x28, 0x09, 0x05, 
	0x39, 0x23, 0xF3, 0x04, 0xC8, 0xCC, 0xD6, 0x6F, 0x16, 0xFA, 
	0x00, 0x20, 0xA3, 0xDC, 0x0E, 0xAA, 0xAA, 0xFF, 0xAB, 0xE2, 
	0x19, 0xF1, 0xB0, 0x5E, 0x18, 0x47, 0x70, 0x4F, 0xF9, 0x8D, 
	0xC7, 0xB9, 0x80, 0xBB, 0xA5, 0x4F, 0xEE, 0xF5, 0xF8, 0x57, 
	0x68, 0x80, 0xE7, 0x91, 0x9A, 0x55, 0x3C, 0x9B, 0xFB, 0xE5, 
	0x5E, 0x13, 0x88, 0x4B, 0x79, 0x81, 0xED, 0xF8, 0xCB, 0xCC, 
	0xCE, 0xFF, 0x7E, 0x34, 0x87, 0x38, 0x66, 0x41, 0xAC, 0x93, 
	0xAC, 0xF6, 0x30, 0x81, 0x4A, 0x8A, 0xDF, 0xE0, 0xAB, 0xC2, 
	0x2F, 0x9C, 0xA5, 0xB3, 0xDC, 0xE4, 0xBC, 0xCE, 0xCB, 0xA0, 
	0xBF, 0xC8, 0xEF, 0x21, 0x49, 0x79, 0x05, 0x17, 0xF0, 0x6C, 
	0x6F, 0xE5, 0x44, 0xA5, 0x9A, 0x5F, 0xE1, 0xEB, 0x02, 0x45, 
	0x76, 0x19, 0xDC, 0x9F, 0x6F, 0xE6, 0xD7, 0xB3, 0x1C, 0xA9, 
	0x71, 0x94, 0xBC, 0x25, 0x77, 0xE4, 0x77, 0xDA, 0x13, 0xEC, 
	0x60, 0xE5, 0xCB, 0xF0, 0x98, 0xDF, 0x43, 0xB5, 0xF5, 0x53, 
	0xBC, 0x4D, 0x6B, 0xF5, 0x03, 0xDA, 0x9A, 0xDA, 0x97, 0xAC, 
	0xB6, 0x85, 0xCB, 0x50, 0xE2, 0x04, 0xEB, 0x64, 0xF4, 0xA1, 
	0x81, 0x3A, 0x14, 0x23, 0xE9, 0x24, 0x7F, 0x32, 0xB1, 0x47, 
	0x27, 0x5B, 0x2B, 0xF3, 0x49, 0x3F, 0x70, 0xC6, 0x22, 0x7F, 
	0x0D, 0x4F, 0x05, 0xCD, 0xF1, 0xD4, 0x5A, 0x2A, 0xD3, 0xBD, 
	0x38, 0x50, 0x7F, 0xB4, 0x76, 0x81, 0xB6, 0xA6, 0x8E, 0xDA, 
	0x19, 0x6D, 0x82, 0x86, 0x56, 0xEB, 0x7C, 0xBD, 0x21, 0xFF, 
	0xC7, 0x38, 0x06, 0xEF, 0x4E, 0x24, 0x3F, 0xD0, 0xE9, 0xD4, 
	0xAC, 0x53, 0x61, 0x00, 0x9F, 0xE8, 0x8D, 0xD6, 0xE2, 0x28, 
	0x04, 0x87, 0x70, 0x74, 0xA5, 0xD6, 0x1B, 0x4F, 0x51, 0x7B, 
	0x0C, 0x6A, 0xB6, 0xD3, 0xC5, 0xAA, 0x71, 0xBF, 0x31, 0xC9, 
	0x8C, 0x28, 0x70, 0x3E, 0xFC, 0x69, 0xCD, 0x83, 0xF4, 0x3E, 
	0xBA, 0x30, 0xCA, 0x18, 0x4E, 0x00, 0x50, 0x8B, 0xE6, 0x47, 
	0xF6, 0x03, 0x0B, 0xB9, 0x19, 0x00, 0x00, 0xB8, 0x14, 0xB7, 
	0xD3, 0xD8, 0xC8, 0x7E, 0x62, 0xC3, 0xA4, 0xE7, 0xF5, 0x5E, 
	0x6B, 0x73, 0xEE, 0x92, 0x22, 0x33, 0x00, 0x00, 0x70, 0x7F, 
	0x4C, 0xC3, 0x65, 0xBE, 0x7F, 0x53, 0xC8, 0x2F, 0xF9, 0x03, 
	0x98, 0x6B, 0xCC, 0xAA, 0xDD, 0x1D, 0x2D, 0xF9, 0xBC, 0x18, 
	0x00, 0x00, 0xA4, 0x48, 0xBF, 0x4F, 0x57, 0x62, 0x78, 0x5E, 
	0xA4, 0x25, 0xB1, 0x42, 0xE7, 0xE2, 0x05, 0xCB, 0x47, 0xE6, 
	0xD2, 0x71, 0x63, 0x80, 0x7A, 0x33, 0x74, 0xC3, 0xA5, 0x98, 
	0x80, 0xD0, 0xE9, 0x14, 0x5A, 0x81, 0x37, 0xB1, 0x04, 0xBF, 
	0xCB, 0xE7, 0x31, 0xED, 0xCD, 0x6A, 0x80, 0x3A, 0x14, 0xB4, 
	0x4E, 0x7D, 0x03, 0xA3, 0xB4, 0x14, 0x83, 0xFD, 0x66, 0x10, 
	0xEB, 0x41, 0x5A, 0x87, 0x77, 0x74, 0x15, 0x56, 0x37, 0x57, 
	0xAB, 0x47, 0x6A, 0x80, 0x46, 0x18, 0xDC, 0xA3, 0xF1, 0xC7, 
	0xD6, 0x4A, 0xB4, 0x3D, 0xDA, 0x50, 0xC3, 0x09, 0xE1, 0x47, 
	0xB4, 0x1C, 0x5F, 0x34, 0xFC, 0xD8, 0x9A, 0x6E, 0xB2, 0x9A, 
	0xE1, 0x49, 0x6F, 0x41, 0x0B, 0x5A, 0xD0, 0x82, 0x16, 0xB4, 
	0xA0, 0x05, 0x2D, 0x68, 0x81, 0x0D, 0xFF, 0x0F, 0x25, 0x3B, 
	0x26, 0xB8, 0x4F, 0x2E, 0x71, 0xC8, 0x00, 0x00, 0x00, 0x25, 
	0x74, 0x45, 0x58, 0x74, 0x64, 0x61, 0x74, 0x65, 0x3A, 0x63, 
	0x72, 0x65, 0x61, 0x74, 0x65, 0x00, 0x32, 0x30, 0x31, 0x34, 
	0x2D, 0x30, 0x38, 0x2D, 0x30, 0x37, 0x54, 0x30, 0x31, 0x3A, 
	0x35, 0x37, 0x3A, 0x32, 0x33, 0x2D, 0x30, 0x37, 0x3A, 0x30, 
	0x30, 0xA4, 0xDE, 0x81, 0xC5, 0x00, 0x00, 0x00, 0x25, 0x74, 
	0x45, 0x58, 0x74, 0x64, 0x61, 0x74, 0x65, 0x3A, 0x6D, 0x6F, 
	0x64, 0x69, 0x66, 0x79, 0x00, 0x32, 0x30, 0x31, 0x34, 0x2D, 
	0x30, 0x38, 0x2D, 0x30, 0x37, 0x54, 0x30, 0x31, 0x3A, 0x35, 
	0x37, 0x3A, 0x32, 0x33, 0x2D, 0x30, 0x37, 0x3A, 0x30, 0x30, 
	0xD5, 0x83, 0x39, 0x79, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 
	0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82, 
};

wxBitmap& icon_bw_png_to_wx_bitmap()
{
	static wxMemoryInputStream memIStream( icon_bw_png, sizeof( icon_bw_png ) );
	static wxImage image( memIStream, wxBITMAP_TYPE_PNG );
	static wxBitmap bmp( image );
	return bmp;
};


#endif //ICON_BW_PNG_H