//
//  Composant5Version.h
//  test
//
//  Created by Long DO on 5/20/15.
//  Copyright (c) 2015 Long DO. All rights reserved.
//

#ifndef test_Composant5Version_h
#define test_Composant5Version_h

#define ComposantVersion_1 1
#define ComposantVersion_2 7
#define ComposantVersion_3 0
#define ComposantVersion_4 0

#define COMPOSANT_VERSION ComposantVersion_1,ComposantVersion_2,ComposantVersion_2,ComposantVersion_4
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define VERSION_TEXT(_version,_build,_interface,_fonction) \
TOSTRING(_version) TOSTRING(.) TOSTRING(_build) TOSTRING(.) TOSTRING(_interface) STRINGIFY(.) TOSTRING(_fonction)
#define COMPOSANT_VERSION_STR VERSION_TEXT(ComposantVersion_1,ComposantVersion_2,ComposantVersion_2,ComposantVersion_4)

#endif



