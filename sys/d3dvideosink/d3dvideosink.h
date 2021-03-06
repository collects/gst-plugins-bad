/* GStreamer
 * Copyright (C) 2010 David Hoyt <dhoyt@hoytsoft.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __D3DVIDEOSINK_H__
#define __D3DVIDEOSINK_H__

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/video/gstvideosink.h>
#include <gst/video/videooverlay.h>
#include <gst/video/navigation.h>

#include <windows.h>
#if defined(__MINGW32__)
# ifndef _OBJC_NO_COM_
#  if defined(__cplusplus) && !defined(CINTERFACE)
#   if defined(__GNUC__) &&  __GNUC__ < 3 && !defined(NOCOMATTRIBUTE)
#    define DECLARE_INTERFACE_IID_(i,b,d) _COM_interface __attribute__((com_interface)) i : public b
#   else
#    define DECLARE_INTERFACE_IID_(i,b,d) _COM_interface i : public b
#   endif
#  elif !defined(DECLARE_INTERFACE_IID_)
#   define DECLARE_INTERFACE_IID_(i,b,d) DECLARE_INTERFACE(i)
#  endif
# endif
# if !defined(__MSABI_LONG)
#  define __MSABI_LONG(x)  x ## l
# endif
#endif
#include <d3d9.h>
#include <d3dx9tex.h>

#include "directx/directx.h"

#ifdef _MSC_VER
#pragma warning( disable : 4090 4024)
#endif

G_BEGIN_DECLS
#define GST_TYPE_D3DVIDEOSINK                     (gst_d3dvideosink_get_type())
#define GST_D3DVIDEOSINK(obj)                     (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_D3DVIDEOSINK,GstD3DVideoSink))
#define GST_D3DVIDEOSINK_CLASS(klass)             (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_D3DVIDEOSINK,GstD3DVideoSinkClass))
#define GST_D3DVIDEOSINK_GET_CLASS(obj)           (GST_D3DVIDEOSINK_CLASS(G_OBJECT_GET_CLASS(obj)))
#define GST_IS_D3DVIDEOSINK(obj)                  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_D3DVIDEOSINK))
#define GST_IS_D3DVIDEOSINK_CLASS(klass)          (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_D3DVIDEOSINK))

typedef struct _GstD3DVideoSink GstD3DVideoSink;
typedef struct _GstD3DVideoSinkClass GstD3DVideoSinkClass;

#define GST_D3DVIDEOSINK_D3D_DEVICE_LOCK(sink) g_mutex_lock (&GST_D3DVIDEOSINK (sink)->d3d_device_lock)
#define GST_D3DVIDEOSINK_D3D_DEVICE_TRYLOCK(sink) g_mutex_trylock (&GST_D3DVIDEOSINK (sink)->d3d_device_lock)
#define GST_D3DVIDEOSINK_D3D_DEVICE_UNLOCK(sink) g_mutex_unlock (&GST_D3DVIDEOSINK (sink)->d3d_device_lock)

struct _GstD3DVideoSink
{
  GstVideoSink sink;

  /* source rectangle */

  GstVideoFormat format;
  GstVideoInfo info;

  gboolean enable_navigation_events;
 
  gboolean keep_aspect_ratio;
  GValue *par; 

  /* If the window is closed, we set this and error out */
  gboolean window_closed;

  /* The video window set through GstXOverlay */
  HWND window_handle;
  
  /* If we created the window, it needs to be closed in ::stop() */
  gboolean is_new_window;

  /* If we create our own window, we run it from another thread */
  GThread *window_thread;
  HANDLE window_created_signal;

  /* If we use an app-supplied window, we need to hook its WNDPROC */
  WNDPROC prevWndProc;
  gboolean is_hooked;

  GMutex d3d_device_lock;
  LPDIRECT3DSURFACE9 d3d_offscreen_surface;
  LPDIRECT3DDEVICE9 d3ddev;
  D3DPRESENT_PARAMETERS d3dpp;

  D3DFORMAT d3dformat;
  D3DFORMAT d3dfourcc;
  D3DTEXTUREFILTERTYPE d3dfiltertype;
};

struct _GstD3DVideoSinkClass
{
  GstVideoSinkClass parent_class;

  gboolean is_directx_supported;
  gint directx_version;
  DirectXAPI *directx_api;
};

GType gst_d3dvideosink_get_type (void);

G_END_DECLS
#endif /* __D3DVIDEOSINK_H__ */
