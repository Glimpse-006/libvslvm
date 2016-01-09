/*
 * Python object definition of the libvslvm physical volume
 *
 * Copyright (C) 2014-2016, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _PYVSLVM_PHYSICAL_VOLUME_H )
#define _PYVSLVM_PHYSICAL_VOLUME_H

#include <common.h>
#include <types.h>

#include "pyvslvm_libvslvm.h"
#include "pyvslvm_python.h"
#include "pyvslvm_volume_group.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyvslvm_physical_volume pyvslvm_physical_volume_t;

struct pyvslvm_physical_volume
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libvslvm physical volume
	 */
	libvslvm_physical_volume_t *physical_volume;

	/* The pyvslvm volume group object
	 */
	pyvslvm_volume_group_t *volume_group_object;
};

extern PyMethodDef pyvslvm_physical_volume_object_methods[];
extern PyTypeObject pyvslvm_physical_volume_type_object;

PyObject *pyvslvm_physical_volume_new(
           libvslvm_physical_volume_t *physical_volume,
           pyvslvm_volume_group_t *volume_group_object );

int pyvslvm_physical_volume_init(
     pyvslvm_physical_volume_t *pyvslvm_physical_volume );

void pyvslvm_physical_volume_free(
      pyvslvm_physical_volume_t *pyvslvm_physical_volume );

#if defined( __cplusplus )
}
#endif

#endif

