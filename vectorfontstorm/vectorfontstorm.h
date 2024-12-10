#pragma once

/// 3D text vectorisation library with UTF8 support
///
/// Debugging defines:
///   DEBUG_VECTORFONTSTORM - show general debugging output and profiling info
///   DEBUG_VECTORFONTSTORM_DETAILED - show detailed data on the segment scale
/// Other defines:
///   VECTORFONTSTORM_DUPLICATE_VERTS - don't share opengl vertices between triangles

#include "manager.h"
#include "point.h"
#include "segment.h"
#include "contour.h"
#include "glyph.h"
#include "font.h"
#include "string.h"
