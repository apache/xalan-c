/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
 * ====================================================================
 * Configuration information for httpd.conf:
 * ====================================================================
	<Location /xslt>
			AddHandler mod_xslt .html
			AddHandler mod_xslt .txt
	</Location>
 * ====================================================================
*/
#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "http_log.h"
#include "util_script.h"
#include "http_main.h"
#include "http_request.h"



#include <xalanc/XalanTransformer/XalanCAPI.h>



static void xslt_init(server_rec *s, pool *p)
{
	ap_add_version_component("mod_xslt/Xalan/1.0");
}



static void xslt_child_init(server_rec *s, pool *p)
{
	XalanInitialize();
}



static void xslt_child_exit(server_rec *s, pool *p)
{
	XalanTerminate(1);
}



typedef struct CONTROL_STRUCT_TAG
{
	request_rec*	r;
	int				fHeaderSent;
} CONTROL_STRUCT;



static CallbackSizeType xalan_output_handler(const char *data, CallbackSizeType length, void *handle)
{
	CONTROL_STRUCT* c = (CONTROL_STRUCT*)handle;

	if (c->fHeaderSent == 0)
	{
		c->fHeaderSent = 1;

		ap_send_http_header(c->r);
	}

	return ap_rwrite(data, length, c->r);
}



static void xalan_flush_handler(void *handle)
{
	CONTROL_STRUCT* c = (CONTROL_STRUCT*)handle;

	ap_rflush(c->r);
}



static int xslt_handler(request_rec *r)
{
	char * mimetype		= NULL;
	char * filename		= NULL;
	char * xmlfilename	= NULL;
	char * xslfilename	= NULL;

	XalanHandle xalan = NULL;

	int i;
	int dot_point;

	int	error = DECLINED;

	CONTROL_STRUCT	control_struct =
	{
		0,
		0
	};

	control_struct.r = r;

	mimetype = ap_pstrcat(r->pool, r->filename, NULL);
	
    /* Find the extension without any assumptions on string.h */
	
    dot_point = 0;
	i = 0;
	
    while (mimetype[i] != '\0') {
        if (mimetype[i] == '.')
             dot_point = i;
        ++i;
    }

    if (dot_point == 0) {

        fprintf(stderr, "Unable to find extension of : %s\n", mimetype);
        r->uri = mimetype;
     
        return DECLINED;
    }

    filename = ap_pstrndup(r->pool, r->filename,dot_point);

	xmlfilename = ap_pstrcat(r->pool,filename,".xml",NULL); 

	xslfilename = ap_pstrcat(r->pool,filename,".xsl",NULL); 
	
	xalan = CreateXalanTransformer();
	
	error = XalanTransformToHandler(xmlfilename, xslfilename, xalan, &control_struct, xalan_output_handler, xalan_flush_handler);

	if(error)
	{
	    const char* const	msg = XalanGetLastError(xalan);

	    fprintf(stderr,"mod_xslt: %s: %s\n", r->filename, msg);

		r->uri = filename;

		return DECLINED;
	}

	DeleteXalanTransformer(xalan);

	return OK;
}



static const handler_rec xslt_handlers[] =
{ 
    {"mod_xslt", xslt_handler},
    {NULL, NULL}
};



module MODULE_VAR_EXPORT xslt_module =
{
    STANDARD_MODULE_STUFF,
    xslt_init,		/* initializer */
    NULL,			/* create per-directory config structure */
    NULL,			/* merge per-directory config structures */
    NULL,			/* create per-server config structure */
    NULL,			/* merge per-server config structures */
    NULL,			/* command table */
    xslt_handlers,	/* handlers */
    NULL,			/* translate_handler */
    NULL,			/* check_user_id */
    NULL,			/* check auth */
    NULL,			/* check access */
    NULL,			/* type_checker */
    NULL,			/* pre-run fixups */
    NULL,			/* logger */
    NULL,			/* header parser */
    xslt_child_init,/* child_init */
    xslt_child_exit,/* child_exit */
    NULL			/* post read-request */
};
