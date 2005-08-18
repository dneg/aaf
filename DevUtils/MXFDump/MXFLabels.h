// Essence container labels

// Generic container (deprecated)
//
MXF_ESSENCE_CONTANER_NODE(0x01, 0x01, "Generic container (deprecated)")
// None yet.

// D-10
//
MXF_ESSENCE_CONTANER_NODE(0x02, 0x01, "D-10")

  // 50Mbps 625/50i
MXF_ESSENCE_CONTANER_LABEL(0x01, 0x01, "50Mbps 625/50i (defined template)")
MXF_ESSENCE_CONTANER_LABEL(0x01, 0x02, "50Mbps 625/50i (extended template)")
MXF_ESSENCE_CONTANER_LABEL(0x01, 0x7f, "50Mbps 625/50i (picture only)")
  // 50Mbps 525/60i
MXF_ESSENCE_CONTANER_LABEL(0x02, 0x01, "50Mbps 525/60i (defined template)")
MXF_ESSENCE_CONTANER_LABEL(0x02, 0x02, "50Mbps 525/60i (extended template)")
MXF_ESSENCE_CONTANER_LABEL(0x02, 0x7f, "50Mbps 525/60i (picture only)")
  // 40Mbps 625/50i
MXF_ESSENCE_CONTANER_LABEL(0x03, 0x01, "40Mbps 625/50i (defined template)")
MXF_ESSENCE_CONTANER_LABEL(0x03, 0x02, "40Mbps 625/50i (extended template)")
MXF_ESSENCE_CONTANER_LABEL(0x03, 0x7f, "40Mbps 625/50i (picture only)")
  // 40Mbps 525/60i
MXF_ESSENCE_CONTANER_LABEL(0x04, 0x01, "40Mbps 525/60i (defined template)")
MXF_ESSENCE_CONTANER_LABEL(0x04, 0x02, "40Mbps 525/60i (extended template)")
MXF_ESSENCE_CONTANER_LABEL(0x04, 0x7f, "40Mbps 525/60i (picture only)")
  // 30Mbps 625/50i
MXF_ESSENCE_CONTANER_LABEL(0x05, 0x01, "30Mbps 625/50i (defined template)")
MXF_ESSENCE_CONTANER_LABEL(0x05, 0x02, "30Mbps 625/50i (extended template)")
MXF_ESSENCE_CONTANER_LABEL(0x05, 0x7f, "30Mbps 625/50i (picture only)")
  // 30Mbps 525/60i
MXF_ESSENCE_CONTANER_LABEL(0x06, 0x01, "30Mbps 525/60i (defined template)")
MXF_ESSENCE_CONTANER_LABEL(0x06, 0x02, "30Mbps 525/60i (extended template)")
MXF_ESSENCE_CONTANER_LABEL(0x06, 0x7f, "30Mbps 525/60i (picture only)")

// DV
//
MXF_ESSENCE_CONTANER_NODE(0x02, 0x02, "DV")

  // 25Mbps 525/60i
MXF_ESSENCE_CONTANER_LABEL(0x0101, "25Mbps 525/60i (frame wrapped)")
MXF_ESSENCE_CONTANER_LABEL(0x4002, "25Mbps 525/60i (clip wrapped)")
MXF_ESSENCE_CONTANER_LABEL(0x4101, "25Mbps 625/50i (frame wrapped)")
MXF_ESSENCE_CONTANER_LABEL(0x4102, "25Mbps 625/50i (clip wrapped)")
  // 50Mbps 525/60i
MXF_ESSENCE_CONTANER_LABEL(0x5001, "50Mbps 525/60i (frame wrapped)")
MXF_ESSENCE_CONTANER_LABEL(0x5002, "50Mbps 525/60i (clip wrapped)")
MXF_ESSENCE_CONTANER_LABEL(0x5101, "50Mbps 625/50i (frame wrapped)")
MXF_ESSENCE_CONTANER_LABEL(0x5102, "50Mbps 625/50i (clip wrapped)")
  // 100Mbps 1080/60i
MXF_ESSENCE_CONTANER_LABEL(0x6001, "100Mbps 1080/60i (frame wrapped)")
MXF_ESSENCE_CONTANER_LABEL(0x6002, "100Mbps 1080/60i (clip wrapped)")
MXF_ESSENCE_CONTANER_LABEL(0x6101, "100Mbps 1080/50i (frame wrapped)")
MXF_ESSENCE_CONTANER_LABEL(0x6102, "100Mbps 1080/50i (clip wrapped)")
  // 100Mbps 720/60p
MXF_ESSENCE_CONTANER_LABEL(0x6201, "100Mbps 720/60p (frame wrapped)")
MXF_ESSENCE_CONTANER_LABEL(0x6202, "100Mbps 720/60p (clip wrapped)")
MXF_ESSENCE_CONTANER_LABEL(0x6301, "100Mbps 720/50p (frame wrapped)")
MXF_ESSENCE_CONTANER_LABEL(0x6302, "100Mbps 720/50p (clip wrapped)")
  // undefined
MXF_ESSENCE_CONTANER_LABEL(0x7f01, "undefined (frame wrapped)")
MXF_ESSENCE_CONTANER_LABEL(0x7f02, "undefined (clip wrapped)")

// D-11
//
MXF_ESSENCE_CONTANER_NODE(0x02, 0x03, "D-11")
// None yet.

// MPEG Elementary Stream
//
MXF_ESSENCE_CONTANER_NODE(0x02, 0x04, "MPEG Elementary Stream")
// None yet

// Uncompressed Picture
//
MXF_ESSENCE_CONTANER_NODE(0x02, 0x05, "Uncompressed Picture")

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

// AES3/BWF
//
MXF_ESSENCE_CONTANER_NODE(0x02, 0x06, "AES3/BWF")

MXF_ESSENCE_CONTAINER_LABEL(0x01, 0x00, "BWF (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x02, 0x00, "BWF (clip wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x03, 0x00, "AES3 (frame wrapped)")
MXF_ESSENCE_CONTAINER_LABEL(0x04, 0x00, "AES3 (clip wrapped)")

// MPEG Packetized Elementary Stream
//
MXF_ESSENCE_CONTANER_NODE(0x02, 0x07, "MPEG Packetized Elementary Stream")
// None yet.

// MPEG Programme Stream
//
MXF_ESSENCE_CONTANER_NODE(0x02, 0x08, "MPEG Programme Stream")
// None yet.

// MPEG Transport Stream
//
MXF_ESSENCE_CONTANER_NODE(0x02, 0x09, "MPEG Transport Stream")
// None yet.
