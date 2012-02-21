/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Test of XalanCAPI.h
 * With Top-Level Parameters
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "xalanc/XalanTransformer/XalanCAPI.h"

int Usage(const char * program)
{
    printf("\nUSAGE: %s xml-file  stylesheet  outfile  [parameters...]\n"
        "   Parameter: -s name \"'string'\"\n"
        "   Parameter: -n name 123.34532\n"
        "   Parameter: -d name \"xmlfile\"\n",
        program);
    return 1;
}


struct {
    char *  ParmType;
    char *  ParmName;
    char *  ParmValue;
} ParamInfo[15];

XalanPSHandle ParamPSHandle[15];

XalanHandle theTransformer;
char *      theXmlFile;
char *      theXslFile;
char *      theOutFile;


int main(int argc, char * argv[])
{

    int i,j;
    int ParamCount = 0;

    if (argc < 4)
        return Usage(argv[0]);

    theXmlFile = argv[1];
    theXslFile = argv[2];
    theOutFile = argv[3];

    for (i=4, j=0; i<argc ;i++)
    {
        if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "-n") || !strcmp(argv[i], "-d"))
        {
            ParamInfo[j].ParmType = argv[i];
            i++;
        }
        else return Usage(argv[0]);

        if (i == argc)
            return Usage(argv[0]);
        ParamInfo[j].ParmName = argv[i];
        i++;
        if (i == argc)
            return Usage(argv[0]);
        ParamInfo[j].ParmValue = argv[i];
        ParamCount = ++j;
    }
            
    XalanInitialize();

    theTransformer = CreateXalanTransformer();
    if (!theTransformer)
    {
        printf("ERROR: Unable to Create Xalan Transformer\n");
        return Usage(argv[0]);
    }

    for (i = 0; i < ParamCount; i++)
    {
        if (!strcmp(ParamInfo[i].ParmType, "-s"))
        {
            XalanSetStylesheetParam(ParamInfo[i].ParmName, 
                ParamInfo[i].ParmValue,
                theTransformer);
        }
        else if (!strcmp(ParamInfo[i].ParmType, "-n"))
        {
            XalanSetStylesheetParamNumber(ParamInfo[i].ParmName,
                strtod(ParamInfo[i].ParmValue, NULL),
                theTransformer);
        }
        else if (!strcmp(ParamInfo[i].ParmType, "-d"))
        {
            if (XalanParseSource(ParamInfo[i].ParmValue,
                theTransformer,
                &ParamPSHandle[i]))
            {
                //ERRPR
                printf("ERROR: Unable to create Nodeset %s\n",
                    ParamInfo[i].ParmValue);
                return Usage(argv[0]);
            }
            XalanSetStylesheetParamNodeset(ParamInfo[i].ParmName,
                ParamPSHandle[i],
                theTransformer);
        }
    }


    if (XalanTransformToFile(theXmlFile, theXslFile, theOutFile, theTransformer))
    {
        //ERROR
        printf("ERROR: Transformation\n%s\n",
             XalanGetLastError(theTransformer));
        return 1;
    }

    for (i=0; i < ParamCount; i++)
    {
        if (!strcmp(ParamInfo[i].ParmType, "-d"))
        {
            XalanDestroyParsedSource(
                ParamPSHandle[i],
                theTransformer);
        }

    }

    XalanClearStylesheetParams(theTransformer);

    DeleteXalanTransformer(theTransformer);

    XalanTerminate(0);
    return 0;
}
