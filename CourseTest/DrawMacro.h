#pragma once

#include "DrawDebugHelpers.h"

#define DRAW_VECTOR(StartLocation, EndLocation, Color, Time) if(GetWorld()) \
{ \
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color, false, Time);	\
	DrawDebugPoint(GetWorld(), EndLocation, 15.f, Color, false, Time); \
}

#define DRAW_SPHERE(Location, Color, Time) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 20, 12, Color, false, Time)

#define DRAW_LINE(StartLocation, EndLocation, Color, Time) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color, false, Time)

#define DRAW_POINT(Location, Color, Time) if(GetWorld()) DrawDebugPoint(GetWorld(), StartLocation, 10, Color, false, Time)
