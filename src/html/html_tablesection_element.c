/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 Bo Yang <struggleyb.nku@gmail.com>
 * Copyright 2014 Rupinder Singh Khokhar<rsk1coder99@gmail.com>
 */
#include <assert.h>
#include <stdlib.h>

#include <dom/html/html_tablesection_element.h>

#include "html/html_document.h"
#include "html/html_tablesection_element.h"

#include "html/html_collection.h"
#include "html/html_element.h"

#include "core/node.h"
#include "core/attr.h"
#include "utils/utils.h"

static struct dom_element_protected_vtable _protect_vtable = {
	{
		DOM_NODE_PROTECT_VTABLE_HTML_TABLE_SECTION_ELEMENT
	},
	DOM_HTML_TABLE_SECTION_ELEMENT_PROTECT_VTABLE
};

/**
 * Create a dom_html_table_section_element object
 *
 * \table_section doc  The document object
 * \table_section ele  The returned element object
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception _dom_html_table_section_element_create(struct dom_html_document *doc,
		dom_string *tag_name, dom_string *namespace, dom_string *prefix,
		struct dom_html_table_section_element **ele)
{
	struct dom_node_internal *node;

	*ele = malloc(sizeof(dom_html_table_section_element));
	if (*ele == NULL)
		return DOM_NO_MEM_ERR;

	/* Set up vtables */
	node = (struct dom_node_internal *) *ele;
	node->base.vtable = &_dom_html_element_vtable;
	node->vtable = &_protect_vtable;

	return _dom_html_table_section_element_initialise(doc, tag_name, namespace, prefix, *ele);
}

/**
 * Initialise a dom_html_table_section_element object
 *
 * \table_section doc  The document object
 * \table_section ele  The dom_html_table_section_element object
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception _dom_html_table_section_element_initialise(struct dom_html_document *doc,
		dom_string *tag_name, dom_string *namespace, dom_string *prefix,
		struct dom_html_table_section_element *ele)
{
	return _dom_html_element_initialise(doc, &ele->base,
			tag_name,
			namespace, prefix);
}

/**
 * Finalise a dom_html_table_section_element object
 *
 * \table_section ele  The dom_html_table_section_element object
 */
void _dom_html_table_section_element_finalise(struct dom_html_table_section_element *ele)
{
	_dom_html_element_finalise(&ele->base);
}

/**
 * Destroy a dom_html_table_section_element object
 *
 * \table_section ele  The dom_html_table_section_element object
 */
void _dom_html_table_section_element_destroy(struct dom_html_table_section_element *ele)
{
	_dom_html_table_section_element_finalise(ele);
	free(ele);
}

/*------------------------------------------------------------------------*/
/* The protected virtual functions */

/* The virtual function used to parse attribute value, see src/core/element.c
 * for detail */
dom_exception _dom_html_table_section_element_parse_attribute(dom_element *ele,
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
void _dom_virtual_html_table_section_element_destroy(dom_node_internal *node)
{
	_dom_html_table_section_element_destroy((struct dom_html_table_section_element *) node);
}

/* The virtual copy function, see src/core/node.c for detail */
dom_exception _dom_html_table_section_element_copy(dom_node_internal *old,
		dom_node_internal **copy)
{
	return _dom_html_element_copy(old, copy);
}

/*-----------------------------------------------------------------------*/
/* API functions */

#define SIMPLE_GET(attr)						\
	dom_exception dom_html_table_section_element_get_##attr(		\
			dom_html_table_section_element *element,			\
			dom_string **attr)					\
{								\
	dom_exception ret;					\
	dom_string *_memo_##attr;				\
	\
	_memo_##attr =						\
	((struct dom_html_document *)			\
	 ((struct dom_node_internal *)element)->owner)-> \
	memoised[hds_##attr];				\
	\
	ret = dom_element_get_attribute(element, _memo_##attr, attr); \
	\
	return ret;						\
}
#define SIMPLE_SET(attr)						\
	dom_exception dom_html_table_section_element_set_##attr(			\
			dom_html_table_section_element *element,			\
			dom_string *attr)					\
{								\
	dom_exception ret;					\
	dom_string *_memo_##attr;				\
	\
	_memo_##attr =						\
	((struct dom_html_document *)			\
	 ((struct dom_node_internal *)element)->owner)-> \
	memoised[hds_##attr];				\
	\
	ret = dom_element_set_attribute(element, _memo_##attr, attr); \
	\
	return ret;						\
}

#define SIMPLE_GET_SET(attr) SIMPLE_GET(attr) SIMPLE_SET(attr)
SIMPLE_GET_SET(align);
SIMPLE_GET_SET(ch);
SIMPLE_GET_SET(ch_off);
SIMPLE_GET_SET(v_align);

/* The callback function for  _dom_html_collection_create*/
bool table_section_callback(struct dom_node_internal *node, void *ctx)
{
	if(node->type == DOM_ELEMENT_NODE &&
			dom_string_caseless_isequal(node->name,
				((dom_html_document *)ctx)->memoised[hds_TR]))
	{
		return true;
	}
	return false;
}

/**
 * Get the rows collection
 *
 * \param element       The dom_html_section_element object
 * \param rows		The Status
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception dom_html_table_section_element_get_rows(
		dom_html_table_section_element *element,
		dom_html_collection **rows)
{
	dom_html_document *doc = (dom_html_document *) ((dom_node_internal *) element)->owner;
	_dom_html_collection_create(doc, (dom_node_internal *)element, 
			table_section_callback, (void *)doc, rows);
	return DOM_NO_ERR;
}

/**
 * Insert Row before the given Index
 *
 * \param element       The dom_html_section_element object
 * \param index         The Index of the Row node to be inserted
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception dom_html_table_section_element_insert_row(
		dom_html_table_section_element *element,
		int32_t index, dom_html_element **newRow) {
	dom_html_document *doc = (dom_html_document *) ((dom_node_internal *) element)->owner;

	dom_node *node; 		/*< The node at the (index)th position*/

	dom_html_collection *rows; 	/*< The collection of rows in input table_section_element*/
	uint32_t len; 			/*< The size of the row collection */
	dom_exception exp;		/*< Variable for getting the exceptions*/
	exp = _dom_html_element_create(doc, doc->memoised[hds_TR], 
			((dom_node_internal *)element)->namespace,
			((dom_node_internal *)element)->prefix,
			newRow);
	if(exp != DOM_NO_ERR)
		return exp;
	
	exp = dom_html_table_section_element_get_rows(element, &rows);
	if(exp != DOM_NO_ERR)
		return exp;

	exp = dom_html_collection_get_length(rows, &len);
	if(exp != DOM_NO_ERR)
		return exp;
	
	if(index < -1 || (uint32_t)index > len) {
		/* Check for index validity */
		return DOM_INDEX_SIZE_ERR;
	} else if((index == -1 || (uint32_t)index == len)
			&&len != 0) {
		dom_html_collection_item(rows,
				len-1, &node);

		dom_node_internal *internal_node = (dom_node_internal *)node; /*< The dom_node_internal row object at the (len-1)th position*/

		((dom_node_internal *) *newRow)->next = internal_node->next;
		((dom_node_internal *) *newRow)->previous = internal_node;
		internal_node->next = (dom_node_internal *)*newRow;
	} else if(len != 0) {
		dom_html_collection_item(rows,
				index, &node);

		dom_node_internal *internal_node = (dom_node_internal *)node; /*< The dom_node_internal row object at the (index)th position*/

		((dom_node_internal *) *newRow)->next = internal_node;
		((dom_node_internal *) *newRow)->previous = internal_node->previous;
		if(internal_node->previous != NULL) {
			(internal_node->previous)->next = (dom_node_internal *)*newRow;
			internal_node->previous = (dom_node_internal *)*newRow;
		}
	}

	/*Adjust parent's pointers*/
	((dom_node_internal *) *newRow)->parent = (dom_node_internal *)element;
	if(index == 0)
		((dom_node_internal *)element)->first_child = (dom_node_internal *)*newRow;
	if((uint32_t)index == len)
		((dom_node_internal *)element)->last_child = (dom_node_internal *)*newRow;
	
	return DOM_NO_ERR;
}

/**
 * Delete Row at given Index
 *
 * \param element       The dom_html_section_element object
 * \param index		The Index of the Row node to be deleted
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception dom_html_table_section_element_delete_row(
		dom_html_table_section_element *element,
		int32_t index) {
	dom_node *node; 		/*< The node at the (index)th position*/

	dom_html_collection *rows; 	/*< The collection of rows in input table_section_element*/
	uint32_t len; 			/*< The size of the row collection */

	dom_exception exp;		/*< Temporary variable to store & check the exceptions*/

	exp = dom_html_table_section_element_get_rows(element, &rows);
	if(exp != DOM_NO_ERR) {
		return exp;
	}

	exp = dom_html_collection_get_length(rows, &len);
	if(exp != DOM_NO_ERR) {
		return exp;
	}

	if(index < -1 || (uint32_t)index >= len) {
		/* Check for index validity */
		return DOM_INDEX_SIZE_ERR;
	} else if(index == -1) {
		exp = dom_html_collection_item(rows,
				len-1, &node);
	} else {
		exp = dom_html_collection_item(rows,
				index, &node);
	}
	if(exp != DOM_NO_ERR)
		return exp;

	dom_node_internal *internal_node = (dom_node_internal *)node;

	/*Fixing the sibling pointers*/
	if(internal_node->previous != NULL) {
		(internal_node->previous)->next = internal_node->next;
	}
	if(internal_node->next != NULL) {
		(internal_node->next)->previous = internal_node->previous;
	}

	_dom_html_element_destroy(internal_node);

	return DOM_NO_ERR;
}

