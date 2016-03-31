/*
 * sgen-minor-scan-object.h: Object scanning in the nursery collectors.
 *
 * Copyright 2001-2003 Ximian, Inc
 * Copyright 2003-2010 Novell, Inc.
 * Copyright (C) 2012 Xamarin Inc
 *
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

extern guint64 stat_scan_object_called_nursery;

#if defined(SGEN_SIMPLE_NURSERY)
#define SERIAL_SCAN_OBJECT simple_nursery_serial_scan_object
#define SERIAL_SCAN_VTYPE simple_nursery_serial_scan_vtype

#elif defined (SGEN_SPLIT_NURSERY)
#define SERIAL_SCAN_OBJECT split_nursery_serial_scan_object
#define SERIAL_SCAN_VTYPE split_nursery_serial_scan_vtype

#else
#error "Please define GC_CONF_NAME"
#endif

#undef HANDLE_PTR
/* Global remsets are handled in SERIAL_COPY_OBJECT_FROM_OBJ */
#define HANDLE_PTR(ptr,obj)	do {	\
		void *__old = *(ptr);	\
		SGEN_OBJECT_LAYOUT_STATISTICS_MARK_BITMAP ((obj), (ptr)); \
		binary_protocol_scan_process_reference ((full_object), (ptr), __old); \
		if (__old) {	\
			SERIAL_COPY_OBJECT_FROM_OBJ ((ptr), queue);	\
			SGEN_COND_LOG (9, __old != *(ptr), "Overwrote field at %p with %p (was: %p)", (ptr), *(ptr), __old); \
		}	\
	} while (0)

static void
SERIAL_SCAN_OBJECT (GCObject *full_object, SgenDescriptor desc, SgenGrayQueue *queue)
{
	char *start = (char*)full_object;

	SGEN_OBJECT_LAYOUT_STATISTICS_DECLARE_BITMAP;

#ifdef HEAVY_STATISTICS
	sgen_descriptor_count_scanned_object (desc);
#endif

	SGEN_ASSERT (9, sgen_get_current_collection_generation () == GENERATION_NURSERY, "Must not use minor scan during major collection.");

#define SCAN_OBJECT_PROTOCOL
#include "sgen-scan-object.h"

	SGEN_OBJECT_LAYOUT_STATISTICS_COMMIT_BITMAP;
	HEAVY_STAT (++stat_scan_object_called_nursery);
}

static void
SERIAL_SCAN_VTYPE (GCObject *full_object, char *start, SgenDescriptor desc, SgenGrayQueue *queue BINARY_PROTOCOL_ARG (size_t size))
{
	SGEN_OBJECT_LAYOUT_STATISTICS_DECLARE_BITMAP;

	SGEN_ASSERT (9, sgen_get_current_collection_generation () == GENERATION_NURSERY, "Must not use minor scan during major collection.");

	/* The descriptors include info about the MonoObject header as well */
	start -= SGEN_CLIENT_OBJECT_HEADER_SIZE;

#define SCAN_OBJECT_NOVTABLE
#define SCAN_OBJECT_PROTOCOL
#include "sgen-scan-object.h"
}

static void
SERIAL_SCAN_PTR_FIELD (GCObject *full_object, GCObject **ptr, SgenGrayQueue *queue)
{
	HANDLE_PTR (ptr, NULL);
}

#define FILL_MINOR_COLLECTOR_SCAN_OBJECT(collector)	do {			\
		(collector)->serial_ops.scan_object = SERIAL_SCAN_OBJECT;	\
		(collector)->serial_ops.scan_vtype = SERIAL_SCAN_VTYPE; \
		(collector)->serial_ops.scan_ptr_field = SERIAL_SCAN_PTR_FIELD;	\
	} while (0)
