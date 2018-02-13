#ifndef IUPDATABLE_H
#define IUPDATABLE_H
class ParticleDrawer;

class IUpdatable
{
public:
	virtual void update(float dt) = 0;
	virtual void init(ParticleDrawer* drawer) =0;
};

#endif