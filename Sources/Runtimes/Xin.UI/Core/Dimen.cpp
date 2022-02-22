#include "PCH.h"
#include "Dimen.h"

namespace Xin::UI
{
	FDimen FDimen::Auto { NaN<dimenx>, EUnit::Auto };
	FDimen FDimen::Percent10  { 0.1, EUnit::Percent };
	FDimen FDimen::Percent20  { 0.2, EUnit::Percent };
	FDimen FDimen::Percent25  { 0.2, EUnit::Percent };
	FDimen FDimen::Percent30  { 0.3, EUnit::Percent };
	FDimen FDimen::Percent40  { 0.4, EUnit::Percent };
	FDimen FDimen::Percent50  { 0.5, EUnit::Percent };
	FDimen FDimen::Percent60  { 0.6, EUnit::Percent };
	FDimen FDimen::Percent70  { 0.7, EUnit::Percent };
	FDimen FDimen::Percent75  { 0.7, EUnit::Percent };
	FDimen FDimen::Percent80  { 0.8, EUnit::Percent };
	FDimen FDimen::Percent90  { 0.9, EUnit::Percent };
	FDimen FDimen::Percent100 { 1.0, EUnit::Percent };
}
