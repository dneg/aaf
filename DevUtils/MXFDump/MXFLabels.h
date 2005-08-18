//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// Essence container labels

// Generic container (deprecated)
//
MXF_ESSENCE_CONTAINER_NODE(0x01, 0x01, "Generic container (deprecated)")
// None yet.
MXF_ESSENCE_CONTAINER_END()

// D-10
//
MXF_ESSENCE_CONTAINER_NODE(0x02, 0x01, "D-10")

  // 50Mbps 625/50i
MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x01, "50Mbps 625/50i (defined template)")
MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x02, "50Mbps 625/50i (extended template)")
MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x7f, "50Mbps 625/50i (picture only)")
  // 50Mbps 525/60i
MXF_ESSENCE_CONTAINER_LABEL(0x02, 0x01, "50Mbps 525/60i (defined template)")
MXF_ESSENCE_CONTAINER_LABEL(0x02, 0x02, "50Mbps 525/60i (extended template)")
MXF_ESSENCE_CONTAINER_LABEL(0x02, 0x7f, "50Mbps 525/60i (picture only)")
  // 40Mbps 625/50i
MXF_ESSENCE_CONTAINER_LABEL(0x03, 0x01, "40Mbps 625/50i (defined template)")
MXF_ESSENCE_CONTAINER_LABEL(0x03, 0x02, "40Mbps 625/50i (extended template)")
MXF_ESSENCE_CONTAINER_LABEL(0x03, 0x7f, "40Mbps 625/50i (picture only)")
  // 40Mbps 525/60i
MXF_ESSENCE_CONTAINER_LABEL(0x04, 0x01, "40Mbps 525/60i (defined template)")
MXF_ESSENCE_CONTAINER_LABEL(0x04, 0x02, "40Mbps 525/60i (extended template)")
MXF_ESSENCE_CONTAINER_LABEL(0x04, 0x7f, "40Mbps 525/60i (picture only)")
  // 30Mbps 625/50i
MXF_ESSENCE_CONTAINER_LABEL(0x05, 0x01, "30Mbps 625/50i (defined template)")
MXF_ESSENCE_CONTAINER_LABEL(0x05, 0x02, "30Mbps 625/50i (extended template)")
MXF_ESSENCE_CONTAINER_LABEL(0x05, 0x7f, "30Mbps 625/50i (picture only)")
  // 30Mbps 525/60i
MXF_ESSENCE_CONTAINER_LABEL(0x06, 0x01, "30Mbps 525/60i (defined template)")
MXF_ESSENCE_CONTAINER_LABEL(0x06, 0x02, "30Mbps 525/60i (extended template)")
MXF_ESSENCE_CONTAINER_LABEL(0x06, 0x7f, "30Mbps 525/60i (picture only)")

MXF_ESSENCE_CONTAINER_END()

// DV
//
MXF_ESSENCE_CONTAINER_NODE(0x02, 0x02, "DV")

  // 25Mbps 525/60i and 625/50i
MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x01, "IEC 25Mbps 525/60i (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x02, "IEC 25Mbps 525/60i (clip wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x02, 0x01, "IEC 25Mbps 625/50i (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x02, 0x02, "IEC 25Mbps 625/50i (clip wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x03, 0x01, "IEC 25Mbps 525/60i DVCAM (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x03, 0x02, "IEC 25Mbps 525/60i DVCAM (clip wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x04, 0x01, "IEC 25Mbps 625/50i DVCAM (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x04, 0x02, "IEC 25Mbps 625/50i DVCAM (clip wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x3f, 0x01, "Undefined IEC DV (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x3f, 0x02, "Undefined IEC DV (clip wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x40, 0x01, "25Mbps 525/60i (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x40, 0x02, "25Mbps 525/60i (clip wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x41, 0x01, "25Mbps 625/50i (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x41, 0x02, "25Mbps 625/50i (clip wrapped)")
  // 50Mbps 525/60i
MXF_ESSENCE_CONTAINER_LABEL(0x50, 0x01, "50Mbps 525/60i (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x50, 0x02, "50Mbps 525/60i (clip wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x51, 0x01, "50Mbps 625/50i (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x51, 0x02, "50Mbps 625/50i (clip wrapped)")
  // 100Mbps 1080/60i
MXF_ESSENCE_CONTAINER_LABEL(0x60, 0x01, "100Mbps 1080/60i (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x60, 0x02, "100Mbps 1080/60i (clip wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x61, 0x01, "100Mbps 1080/50i (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x61, 0x02, "100Mbps 1080/50i (clip wrapped)")
  // 100Mbps 720/60p
MXF_ESSENCE_CONTAINER_LABEL(0x62, 0x01, "100Mbps 720/60p (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x62, 0x02, "100Mbps 720/60p (clip wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x63, 0x01, "100Mbps 720/50p (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x63, 0x02, "100Mbps 720/50p (clip wrapped)")
  // undefined
MXF_ESSENCE_CONTAINER_LABEL(0x7f, 0x01, "undefined (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x7f, 0x02, "undefined (clip wrapped)")

MXF_ESSENCE_CONTAINER_END()

// D-11
//
MXF_ESSENCE_CONTAINER_NODE(0x02, 0x03, "D-11")
// None yet.
MXF_ESSENCE_CONTAINER_END()

// MPEG Elementary Stream
//
MXF_ESSENCE_CONTAINER_NODE(0x02, 0x04, "MPEG Elementary Stream")

  // ISO/IEC 13818-1 stream id (0x00 - 0x70) (frame wrapped)
//MXF_ESSENCE_CONTAINER_LABEL(0xXX, 0x01, "stream id 0xXX (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x60, 0x01, "stream id 0x60 (frame wrapped)")

  // ISO/IEC 13818-1 stream id (0x00 - 0x70) (clip wrapped)
//MXF_ESSENCE_CONTAINER_LABEL(0xXX, 0x02, "stream id 0xXX (clip wrapped)")

MXF_ESSENCE_CONTAINER_END()

// Uncompressed Picture
//
MXF_ESSENCE_CONTAINER_NODE(0x02, 0x05, "Uncompressed Picture")

MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x01, "525 60i 422 13.5MHz (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x02, "525 60i 422 13.5MHz (clip wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x03, "525 60i 422 13.5MHz (line wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x05, "625 50i 422 13.5MHz (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x06, "625 50i 422 13.5MHz (clip wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x07, "625 50i 422 13.5MHz (line wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x19, "525 60p 422 27MHz (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x1A, "525 60p 422 27MHz (clip wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x1B, "525 60p 422 27MHz (line wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x1D, "625 50p 422 27MHz (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x1E, "625 50p 422 27MHz (clip wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x1F, "625 50p 422 27MHz (line wrapped)")

MXF_ESSENCE_CONTAINER_END()

// AES3/BWF
//
MXF_ESSENCE_CONTAINER_NODE(0x02, 0x06, "AES3/BWF")

MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x00, "BWF (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x02, 0x00, "BWF (clip wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x03, 0x00, "AES3 (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x04, 0x00, "AES3 (clip wrapped)")

MXF_ESSENCE_CONTAINER_END()

// MPEG Packetized Elementary Stream
//
MXF_ESSENCE_CONTAINER_NODE(0x02, 0x07, "MPEG Packetized Elementary Stream")
// None yet.
MXF_ESSENCE_CONTAINER_END()

// MPEG Programme Stream
//
MXF_ESSENCE_CONTAINER_NODE(0x02, 0x08, "MPEG Programme Stream")
// None yet.
MXF_ESSENCE_CONTAINER_END()

// MPEG Transport Stream
//
MXF_ESSENCE_CONTAINER_NODE(0x02, 0x09, "MPEG Transport Stream")
// None yet.
MXF_ESSENCE_CONTAINER_END()

// A-Law sound element mapping
//
MXF_ESSENCE_CONTAINER_NODE(0x02, 0x0A, "A-Law Sound Element")

MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x00, "A-Law Audio (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x02, 0x00, "A-Law Audio (clip wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x03, 0x00, "A-Law Audio (custom wrapped)")

MXF_ESSENCE_CONTAINER_END()
