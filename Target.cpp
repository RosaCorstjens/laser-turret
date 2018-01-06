#include "Target.h"

Target::Target(){}

Target::Target(int orienHor, int orienVer) : orienHor(orienHor), orienVer(orienVer) {}

Target::~Target() {}

bool Target::ForwardOnAxis(int currOrien, bool axis) {
	int targetOrien = axis ? GetOrienHor() : GetOrienVer();

	if (currOrien < targetOrien) {
		return true;
	}
	if (currOrien > targetOrien) {
		return false;
	}
}
