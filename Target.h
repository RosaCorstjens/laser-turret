#ifndef _TARGET_h
#define _TARGET_h

class Target {
public:
	Target();
	Target(int orienHor, int orienVer);
	~Target();

	int GetOrienHor() { return orienHor; }
	int GetOrienVer() { return orienVer; }

	bool ForwardOnAxis(int currOrien, bool axis);

private:
	int orienHor, orienVer = 0;
};

#endif