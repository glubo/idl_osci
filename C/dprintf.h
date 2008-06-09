#pragma once
#ifndef DPRINTF_H_INCLUDED
#define DPRINTF_H_INCLUDED
int debug=1;
#define dprintf if(debug)printf 
#endif
