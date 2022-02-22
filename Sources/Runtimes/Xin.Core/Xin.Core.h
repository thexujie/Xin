#pragma once

#include "Xin.Core.Types.h"

#include "Core/NamedValue.h"

#include "Core/Freezable.h"
#include "Core/Debug.h"
#include "Core/Module.h"
#include "Core/FileLinePeriod.h"
#include "Core/Logger.h"
#include "Core/Thread.h"
#include "Core/DispatcherTimer.h"
#include "Core/Dispatcher.h"
#include "Core/Procedurer.h"

#include "Core/Xml.h"
#include "Core/Ini.h"
#include "Core/Json.h"

#include "Core/Regex.h"
#include "Core/ModuleInstance.h"
#include "Core/Counter.h"

#include "Core/Application.h"

#include "Threading/Mutex.h"
#include "Threading/Signal.h"
#include "Threading/ConditionalVariable.h"
#include "Threading/Lock.h"

#include "Media/Color.h"
#include "Media/Brush.h"
#include "Media/Pen.h"

#include "X2D/Transform3x2.h"
#include "X2D/Font.h"
#include "X2D/TextLayout.h"
#include "X2D/Bitmap.h"
#include "X2D/Path.h"
#include "X2D/Painter.h"
#include "X2D/Graphics.h"

#include "X3D/BoundingBox.h"

#include "Media/Animation/Animatable.h"

#include "Platform/Desktop.h"

#ifdef _WINDOWS_
#error include for <windows.h> is not possible in modulde 'Core'.
#endif

namespace Xin
{
}
