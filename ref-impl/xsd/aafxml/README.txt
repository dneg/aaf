                            AAF-XML Schema Generation
                                    July 2008


This directory contains a set of stylesheets for generating W3C XML Schemas
(XSD) from an AAF-XML document and baseline meta-dictionary XML document. The
generated XSDs can be used to validate the AAF-XML document.

The baseline meta-dictionary is defined in metadict-20050803.xml. This file must
be kept in sync with the baseline set in the AAF SDK C++ library. See the end of
the AAF/ref-impl/src/OM/OMSymbolspace.cpp source file for the list of
meta-definitions included in the baseline.

XSDs can be generated using the build-schema.sh/bat script. This script assumes
the saxon.jar (http://saxon.sourceforge.net, version 6.5.3) XSLT processing
library is present in the current directory. Newer versions of the Saxon library
or other stylesheet processing libraries can also be used.

The generation of the XSDs happens in 3 stages: 1) merge the baseline
meta-dictionary and the in-file extensions into a single meta-dictionary, 2)
preprocess the meta-dictionary resulting from step 1 for accessing in the last 
step, 3) generate the XSD.

Run the script with parameters: <meta-dictionary filename> <aaf-xml filename>,
eg. 
    ./build-schema.sh metadict-20050803.xml example/example.xml
This produces a baseline XSD, aaf.xsd, and zero or more extension schemas. The
example/ directory contains an example.xml document and the generated XSDs
aaf.xsd and ns1.xsd. Note that the stream files, out_streams/stream0 and
out_streams/stream1 are not included.

You can validate the example AAF-XML document, eg. using Xerces
(http://xerces.apache.org/), by copying metadict.xsd into the example/ directory
and adding the following attributes to the <AAF> root element
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://www.aafassociation.org/aafx/v1.1/20050628 aaf.xsd"

