/* ====================================================================
 * Copyright (c) 1995-1999 The Apache Group.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the Apache Group
 *    for use in the Apache HTTP server project (http://www.apache.org/)."
 *
 * 4. The names "Apache Server" and "Apache Group" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache"
 *    nor may "Apache" appear in their names without prior written
 *    permission of the Apache Group.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the Apache Group
 *    for use in the Apache HTTP server project (http://www.apache.org/)."
 *
 * THIS SOFTWARE IS PROVIDED BY THE APACHE GROUP ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE APACHE GROUP OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Group and was originally based
 * on public domain software written at the National Center for
 * Supercomputing Applications, University of Illinois, Urbana-Champaign.
 * For more information on the Apache Group and the Apache HTTP server
 * project, please see <http://www.apache.org/>.
 *
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
