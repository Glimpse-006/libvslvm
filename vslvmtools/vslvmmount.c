/*
 * Mounts a Linux Logical Volume Manager (LVM) volume system
 *
 * Copyright (C) 2014-2023, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#include <stdio.h>

#if defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#include "mount_dokan.h"
#include "mount_fuse.h"
#include "mount_handle.h"
#include "vslvmtools_getopt.h"
#include "vslvmtools_i18n.h"
#include "vslvmtools_libcerror.h"
#include "vslvmtools_libclocale.h"
#include "vslvmtools_libcnotify.h"
#include "vslvmtools_libvslvm.h"
#include "vslvmtools_output.h"
#include "vslvmtools_signal.h"
#include "vslvmtools_unused.h"

mount_handle_t *vslvmmount_mount_handle = NULL;
int vslvmmount_abort                    = 0;

/* Prints usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use vslvmmount to mount a Linux Logical Volume Manager (LVM) volume system\n\n" );

	fprintf( stream, "Usage: vslvmmount [ -o offset ] [ -X extended_options ] [ -hvV ] volume\n"
	                 "                  mount_point\n\n" );

	fprintf( stream, "\tvolume:      a Linux Logical Volume Manager (LVM) volume system\n\n" );
	fprintf( stream, "\tmount_point: the directory to serve as mount point\n\n" );

	fprintf( stream, "\t-h:          shows this help\n" );
	fprintf( stream, "\t-o:          specify the volume offset in bytes\n" );
	fprintf( stream, "\t-v:          verbose output to stderr, while vslvmmount will remain running in the\n"
	                 "\t             foreground\n" );
	fprintf( stream, "\t-V:          print version\n" );
	fprintf( stream, "\t-X:          extended options to pass to sub system\n" );
}

/* Signal handler for vslvmmount
 */
void vslvmmount_signal_handler(
      vslvmtools_signal_t signal VSLVMTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "vslvmmount_signal_handler";

	VSLVMTOOLS_UNREFERENCED_PARAMETER( signal )

	vslvmmount_abort = 1;

	if( vslvmmount_mount_handle != NULL )
	{
		if( mount_handle_signal_abort(
		     vslvmmount_mount_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal mount handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libvslvm_error_t *error                     = NULL;
	system_character_t *mount_point             = NULL;
	system_character_t *option_extended_options = NULL;
	system_character_t *option_offset           = NULL;
	const system_character_t *path_prefix       = NULL;
	system_character_t *source                  = NULL;
	char *program                               = "vslvmmount";
	system_integer_t option                     = 0;
	size_t path_prefix_size                     = 0;
	int result                                  = 0;
	int verbose                                 = 0;

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	struct fuse_operations vslvmmount_fuse_operations;

	struct fuse_args vslvmmount_fuse_arguments  = FUSE_ARGS_INIT(0, NULL);
	struct fuse_chan *vslvmmount_fuse_channel   = NULL;
	struct fuse *vslvmmount_fuse_handle         = NULL;

#elif defined( HAVE_LIBDOKAN )
	DOKAN_OPERATIONS vslvmmount_dokan_operations;
	DOKAN_OPTIONS vslvmmount_dokan_options;
#endif

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
             "vslvmtools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
	if( vslvmtools_output_initialize(
             _IONBF,
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	vslvmtools_output_version_fprint(
	 stdout,
	 program );

	while( ( option = vslvmtools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "ho:vVX:" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'o':
				option_offset = optarg;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				vslvmtools_output_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'X':
				option_extended_options = optarg;

				break;
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source volume.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source = argv[ optind++ ];

	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing mount point.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	mount_point = argv[ optind ];

	libcnotify_verbose_set(
	 verbose );
	libvslvm_notify_set_stream(
	 stderr,
	 NULL );
	libvslvm_notify_set_verbose(
	 verbose );

	if( mount_handle_initialize(
	     &vslvmmount_mount_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize mount handle.\n" );

		goto on_error;
	}
	if( option_offset != NULL )
	{
		if( mount_handle_set_offset(
		     vslvmmount_mount_handle,
		     option_offset,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set volume offset.\n" );

			goto on_error;
		}
	}
#if defined( WINAPI )
	path_prefix = _SYSTEM_STRING( "\\LVM" );
#else
	path_prefix = _SYSTEM_STRING( "/lvm" );
#endif
	path_prefix_size = 1 + system_string_length(
	                        path_prefix );

	if( mount_handle_set_path_prefix(
	     vslvmmount_mount_handle,
	     path_prefix,
	     path_prefix_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set path prefix.\n" );

		goto on_error;
	}
	if( mount_handle_open(
	     vslvmmount_mount_handle,
	     source,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open: %" PRIs_SYSTEM "\n",
		 source );

		goto on_error;
	}
#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	if( option_extended_options != NULL )
	{
		/* This argument is required but ignored
		 */
		if( fuse_opt_add_arg(
		     &vslvmmount_fuse_arguments,
		     "" ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
		if( fuse_opt_add_arg(
		     &vslvmmount_fuse_arguments,
		     "-o" ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
		if( fuse_opt_add_arg(
		     &vslvmmount_fuse_arguments,
		     option_extended_options ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
	}
	if( memory_set(
	     &vslvmmount_fuse_operations,
	     0,
	     sizeof( struct fuse_operations ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear fuse operations.\n" );

		goto on_error;
	}
	vslvmmount_fuse_operations.open       = &mount_fuse_open;
	vslvmmount_fuse_operations.read       = &mount_fuse_read;
	vslvmmount_fuse_operations.release    = &mount_fuse_release;
	vslvmmount_fuse_operations.opendir    = &mount_fuse_opendir;
	vslvmmount_fuse_operations.readdir    = &mount_fuse_readdir;
	vslvmmount_fuse_operations.releasedir = &mount_fuse_releasedir;
	vslvmmount_fuse_operations.getattr    = &mount_fuse_getattr;
	vslvmmount_fuse_operations.destroy    = &mount_fuse_destroy;

	vslvmmount_fuse_channel = fuse_mount(
	                           mount_point,
	                           &vslvmmount_fuse_arguments );

	if( vslvmmount_fuse_channel == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to create fuse channel.\n" );

		goto on_error;
	}
	vslvmmount_fuse_handle = fuse_new(
	                          vslvmmount_fuse_channel,
	                          &vslvmmount_fuse_arguments,
	                          &vslvmmount_fuse_operations,
	                          sizeof( struct fuse_operations ),
	                          vslvmmount_mount_handle );
	
	if( vslvmmount_fuse_handle == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to create fuse handle.\n" );

		goto on_error;
	}
	if( verbose == 0 )
	{
		if( fuse_daemonize(
		     0 ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable to daemonize fuse.\n" );

			goto on_error;
		}
	}
	result = fuse_loop(
	          vslvmmount_fuse_handle );

	if( result != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to run fuse loop.\n" );

		goto on_error;
	}
	fuse_destroy(
	 vslvmmount_fuse_handle );

	fuse_opt_free_args(
	 &vslvmmount_fuse_arguments );

	return( EXIT_SUCCESS );

#elif defined( HAVE_LIBDOKAN )
	if( memory_set(
	     &vslvmmount_dokan_operations,
	     0,
	     sizeof( DOKAN_OPERATIONS ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear dokan operations.\n" );

		goto on_error;
	}
	if( memory_set(
	     &vslvmmount_dokan_options,
	     0,
	     sizeof( DOKAN_OPTIONS ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear dokan options.\n" );

		goto on_error;
	}
	vslvmmount_dokan_options.Version     = DOKAN_VERSION;
	vslvmmount_dokan_options.ThreadCount = 0;
	vslvmmount_dokan_options.MountPoint  = mount_point;

	if( verbose != 0 )
	{
		vslvmmount_dokan_options.Options |= DOKAN_OPTION_STDERR;
#if defined( HAVE_DEBUG_OUTPUT )
		vslvmmount_dokan_options.Options |= DOKAN_OPTION_DEBUG;
#endif
	}
/* This will only affect the drive properties
	vslvmmount_dokan_options.Options |= DOKAN_OPTION_REMOVABLE;
*/

#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
	vslvmmount_dokan_options.Options |= DOKAN_OPTION_KEEP_ALIVE;

	vslvmmount_dokan_operations.CreateFile           = &mount_dokan_CreateFile;
	vslvmmount_dokan_operations.OpenDirectory        = &mount_dokan_OpenDirectory;
	vslvmmount_dokan_operations.CreateDirectory      = NULL;
	vslvmmount_dokan_operations.Cleanup              = NULL;
	vslvmmount_dokan_operations.CloseFile            = &mount_dokan_CloseFile;
	vslvmmount_dokan_operations.ReadFile             = &mount_dokan_ReadFile;
	vslvmmount_dokan_operations.WriteFile            = NULL;
	vslvmmount_dokan_operations.FlushFileBuffers     = NULL;
	vslvmmount_dokan_operations.GetFileInformation   = &mount_dokan_GetFileInformation;
	vslvmmount_dokan_operations.FindFiles            = &mount_dokan_FindFiles;
	vslvmmount_dokan_operations.FindFilesWithPattern = NULL;
	vslvmmount_dokan_operations.SetFileAttributes    = NULL;
	vslvmmount_dokan_operations.SetFileTime          = NULL;
	vslvmmount_dokan_operations.DeleteFile           = NULL;
	vslvmmount_dokan_operations.DeleteDirectory      = NULL;
	vslvmmount_dokan_operations.MoveFile             = NULL;
	vslvmmount_dokan_operations.SetEndOfFile         = NULL;
	vslvmmount_dokan_operations.SetAllocationSize    = NULL;
	vslvmmount_dokan_operations.LockFile             = NULL;
	vslvmmount_dokan_operations.UnlockFile           = NULL;
	vslvmmount_dokan_operations.GetFileSecurity      = NULL;
	vslvmmount_dokan_operations.SetFileSecurity      = NULL;
	vslvmmount_dokan_operations.GetDiskFreeSpace     = NULL;
	vslvmmount_dokan_operations.GetVolumeInformation = &mount_dokan_GetVolumeInformation;
	vslvmmount_dokan_operations.Unmount              = &mount_dokan_Unmount;

#else
	vslvmmount_dokan_operations.ZwCreateFile         = &mount_dokan_ZwCreateFile;
	vslvmmount_dokan_operations.Cleanup              = NULL;
	vslvmmount_dokan_operations.CloseFile            = &mount_dokan_CloseFile;
	vslvmmount_dokan_operations.ReadFile             = &mount_dokan_ReadFile;
	vslvmmount_dokan_operations.WriteFile            = NULL;
	vslvmmount_dokan_operations.FlushFileBuffers     = NULL;
	vslvmmount_dokan_operations.GetFileInformation   = &mount_dokan_GetFileInformation;
	vslvmmount_dokan_operations.FindFiles            = &mount_dokan_FindFiles;
	vslvmmount_dokan_operations.FindFilesWithPattern = NULL;
	vslvmmount_dokan_operations.SetFileAttributes    = NULL;
	vslvmmount_dokan_operations.SetFileTime          = NULL;
	vslvmmount_dokan_operations.DeleteFile           = NULL;
	vslvmmount_dokan_operations.DeleteDirectory      = NULL;
	vslvmmount_dokan_operations.MoveFile             = NULL;
	vslvmmount_dokan_operations.SetEndOfFile         = NULL;
	vslvmmount_dokan_operations.SetAllocationSize    = NULL;
	vslvmmount_dokan_operations.LockFile             = NULL;
	vslvmmount_dokan_operations.UnlockFile           = NULL;
	vslvmmount_dokan_operations.GetFileSecurity      = NULL;
	vslvmmount_dokan_operations.SetFileSecurity      = NULL;
	vslvmmount_dokan_operations.GetDiskFreeSpace     = NULL;
	vslvmmount_dokan_operations.GetVolumeInformation = &mount_dokan_GetVolumeInformation;
	vslvmmount_dokan_operations.Unmounted            = NULL;
	vslvmmount_dokan_operations.FindStreams          = NULL;
	vslvmmount_dokan_operations.Mounted              = NULL;

#endif /* ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 ) */

	result = DokanMain(
	          &vslvmmount_dokan_options,
	          &vslvmmount_dokan_operations );

	switch( result )
	{
		case DOKAN_SUCCESS:
			break;

		case DOKAN_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: generic error\n" );
			break;

		case DOKAN_DRIVE_LETTER_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: bad drive letter\n" );
			break;

		case DOKAN_DRIVER_INSTALL_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: unable to load driver\n" );
			break;

		case DOKAN_START_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: driver error\n" );
			break;

		case DOKAN_MOUNT_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: unable to assign drive letter\n" );
			break;

		case DOKAN_MOUNT_POINT_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: mount point error\n" );
			break;

		default:
			fprintf(
			 stderr,
			 "Unable to run dokan main: unknown error: %d\n",
			 result );
			break;
	}
	return( EXIT_SUCCESS );

#else
	fprintf(
	 stderr,
	 "No sub system to mount LVM format.\n" );

	return( EXIT_FAILURE );
#endif

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	if( vslvmmount_fuse_handle != NULL )
	{
		fuse_destroy(
		 vslvmmount_fuse_handle );
	}
	fuse_opt_free_args(
	 &vslvmmount_fuse_arguments );
#endif
	if( vslvmmount_mount_handle != NULL )
	{
		mount_handle_free(
		 &vslvmmount_mount_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

