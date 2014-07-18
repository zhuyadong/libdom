/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 Bo Yang <struggleyb.nku.com>
 */

#include <stdlib.h>

#include "html/html_style_element.h"
#include "html/html_document.h"

#include "core/node.h"
#include "utils/utils.h"

static struct dom_element_protected_vtable _protect_vtable = {
	{
		DOM_NODE_PROTECT_VTABLE_HTML_STYLE_ELEMENT
	},
	DOM_HTML_STYLE_ELEMENT_PROTECT_VTABLE
};

/**
 * Create a dom_html_style_element object
 *
 * \param doc  The document object
 * \param ele  The returned element object
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception _dom_html_style_element_create(struct dom_html_document *doc,
		struct dom_html_style_element **ele)
{
	struct dom_node_internal *node;

	*ele = malloc(sizeof(dom_html_style_element));
	if (*ele == NULL)
		return DOM_NO_MEM_ERR;

	/* Set up vtables */
	node = (struct dom_node_internal *) *ele;
	node->base.vtable = &_dom_html_element_vtable;
	node->vtable = &_protect_vtable;

	return _dom_html_style_element_initialise(doc, *ele);
}

/**
 * Initialise a dom_html_style_element object
 *
 * \param doc  The document object
 * \param ele  The dom_html_style_element object
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception _dom_html_style_element_initialise(struct dom_html_document *doc,
		struct dom_html_style_element *ele)
{
	dom_string *name = NULL;
	dom_string *media_default = NULL;
	dom_exception err;

	err = dom_string_create((const uint8_t *) "STYLE", SLEN("STYLE"),
			&name);
	if (err != DOM_NO_ERR)
		return err;

	err = dom_string_create((const uint8_t *) "screen", SLEN("screen"),
			&media_default);
	if (err != DOM_NO_ERR)
		return err;
	
	ele->media = media_default;
	err = _dom_html_element_initialise(doc, &ele->base, name, NULL, NULL);
	dom_string_unref(name);

	return err;
}

/**
 * Finalise a dom_html_style_element object
 *
 * \param ele  The dom_html_style_element object
 */
void _dom_html_style_element_finalise(struct dom_html_style_element *ele)
{
	_dom_html_element_finalise(&ele->base);
}

/**
 * Destroy a dom_html_style_element object
 *
 * \param ele  The dom_html_style_element object
 */
void _dom_html_style_element_destroy(struct dom_html_style_element *ele)
{
	_dom_html_style_element_finalise(ele);
	free(ele);
}

/*------------------------------------------------------------------------*/
/* The protected virtual functions */

/* The virtual function used to parse attribute value, see src/core/element.c
 * for detail */
dom_exception _dom_html_style_element_parse_attribute(dom_element *ele,
		dom_string *name, dom_string *value,
		dom_string **parsed)
{
	UNUSED(ele);
	UNUSED(name);

	dom_string_ref(value);
	*parsed = value;

	return DOM_NO_ERR;
}

/* The virtual destroy function, see src/core/node.c for detail */
void _dom_virtual_html_style_element_destroy(dom_node_internal *node)
{
	_dom_html_style_element_destroy((struct dom_html_style_element *) node);
}

/* The virtual copy function, see src/core/node.c for detail */
dom_exception _dom_html_style_element_copy(dom_node_internal *old,
		dom_node_internal **copy)
{
	return _dom_html_element_copy(old, copy);
}


/*-----------------------------------------------------------------------*/
/* Public APIs */

#define SIMPLE_GET(attr)						\
	dom_exception dom_html_style_element_get_##attr(		\
		dom_html_style_element *element,			\
		dom_string **attr)					\
	{								\
		dom_exception ret;					\
		dom_string *_memo_##attr;				\
									\
		_memo_##attr =						\
			((struct dom_html_document *)			\
			 ((struct dom_node_internal *)element)->owner)->\
			memoised[hds_##attr];				\
									\
		ret = dom_element_get_attribute(element, _memo_##attr, attr); \
									\
		return ret;						\
	}
#define SIMPLE_SET(attr)						\
dom_exception dom_html_style_element_set_##attr(			\
		dom_html_style_element *element,			\
		dom_string *attr)					\
	{								\
		dom_exception ret;					\
		dom_string *_memo_##attr;				\
									\
		_memo_##attr =						\
			((struct dom_html_document *)			\
			 ((struct dom_node_internal *)element)->owner)->\
			memoised[hds_##attr];				\
									\
		ret = dom_element_set_attribute(element, _memo_##attr, attr); \
									\
		return ret;						\
	}

#define SIMPLE_GET_SET(attr) SIMPLE_GET(attr) SIMPLE_SET(attr)
SIMPLE_GET_SET(type);
SIMPLE_SET(media);

/**
 * Get the disabled property
 *
 * \param ele       The dom_html_style_element object
 * \param disabled  The returned status
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception dom_html_style_element_get_disabled(dom_html_style_element *ele,
		bool *disabled)
{
	return dom_html_element_get_bool_property(&ele->base, "disabled",
			SLEN("disabled"), disabled);
}

/**
 * Set the disabled property
 *
 * \param ele       The dom_html_style_element object
 * \param disabled  The status
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception dom_html_style_element_set_disabled(dom_html_style_element *ele,
		bool disabled)
{
	return dom_html_element_set_bool_property(&ele->base, "disabled",
			SLEN("disabled"), disabled);
}

/**
 * Get the media property
 *
 * \param ele       The dom_html_style_element object
 * \param media  The returned status
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception dom_html_style_element_get_media(dom_html_style_element *ele,
		dom_string **media)
{
	dom_html_document *doc;
	bool has_value = false;
	dom_exception err;
	 
	doc = (dom_html_document *) ((dom_node_internal *) ele)->owner;

	err = dom_element_has_attribute(ele,
			 doc->memoised[hds_media], &has_value);
	if(err !=DOM_NO_ERR)
		return err;

	if(has_value) {
		return dom_element_get_attribute(ele,
				doc->memoised[hds_media], media);
	}

	*media = ele->media;
	if (*media != NULL)
		dom_string_ref(*media);
	return DOM_NO_ERR;

}

