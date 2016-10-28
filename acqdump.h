#pragma once

#include "../def.h"
#include "acqstring.h"

ACQ_NAMESPACE_START

class dump_style_t
{
};

ACQ_DLL_FUN_DECLEAR(int) dump(const buffer_t&, string_t&, dump_style_t&);

ACQ_NAMESPACE_END