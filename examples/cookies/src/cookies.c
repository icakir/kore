/*
 * Copyright (c) 2017 Stanislav Yudin <stan@endlessinsomnia.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <kore/kore.h>
#include <kore/http.h>

static char 	*html = "<html><body><h1>Reload this page</h1></body></html>";

int		serve_cookies(struct http_request *);

int
serve_cookies(struct http_request *req)
{
	char 			*value;
	struct http_cookie	*cookie;

	http_populate_cookies(req);

	if (http_request_cookie(req, "Simple", &value))
		kore_log(LOG_DEBUG, "Got simple: %s", value);
	if (http_request_cookie(req, "Complex", &value))
		kore_log(LOG_DEBUG, "Got complex: %s", value);
	if (http_request_cookie(req, "Formatted", &value))
		kore_log(LOG_DEBUG, "Got formatted: %s", value);

	/* set simple cookie */
	http_response_cookie(req, "Simple", "Hello World!", 0);

	/* set complex cookie */
	cookie = http_response_cookie(req, "Complex", "Secure Value!",
	    HTTP_COOKIE_HTTPONLY | HTTP_COOKIE_SECURE);
	cookie ->path = kore_strdup("/secure");
	cookie->expires = time(NULL) + 1 * 60 * 60;
	cookie->domain = kore_strdup(req->host);

	/* set formatted cookie */
	http_response_header(req, "set-cookie",
	    "Formatted=TheValue; Path=/vault; HttpOnly");

	http_response(req, 200, html, strlen(html));

	return (KORE_RESULT_OK);
}
