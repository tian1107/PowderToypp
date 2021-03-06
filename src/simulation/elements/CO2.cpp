#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_CO2 PT_CO2 80
Element_CO2::Element_CO2()
{
    Identifier = "DEFAULT_PT_CO2";
    Name = "CO2";
    Colour = PIXPACK(0x666666);
    MenuVisible = 1;
    MenuSection = SC_GAS;
    Enabled = 1;
    
    Advection = 2.0f;
    AirDrag = 0.00f * CFDS;
    AirLoss = 0.99f;
    Loss = 0.30f;
    Collision = -0.1f;
    Gravity = 0.1f;
    Diffusion = 1.0f;
    HotAir = 0.000f	* CFDS;
    Falldown = 1;
    
    Flammable = 0;
    Explosive = 0;
    Meltable = 0;
    Hardness = 0;
    
    Weight = 1;
    
    Temperature = R_TEMP+273.15f;
    HeatConduct = 88;
    Description = "Carbon Dioxide";
    
    State = ST_GAS;
    Properties = TYPE_GAS;
    
    LowPressure = IPL;
    LowPressureTransition = NT;
    HighPressure = IPH;
    HighPressureTransition = NT;
    LowTemperature = 194.65f;
    LowTemperatureTransition = PT_DRIC;
    HighTemperature = ITH;
    HighTemperatureTransition = NT;
    
    Update = &Element_CO2::update;
    
}

//#TPT-Directive ElementHeader Element_CO2 static int update(UPDATE_FUNC_ARGS)
int Element_CO2::update(UPDATE_FUNC_ARGS)
 {
	int r, rx, ry;
	for (rx=-2; rx<3; rx++)
		for (ry=-2; ry<3; ry++)
			if (x+rx>=0 && y+ry>0 && x+rx<XRES && y+ry<YRES && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
                if (parts[i].ctype==5 && 20>(rand()%40000))
				{
					if (sim->create_part(-1, x+rx, y+ry, PT_WATR)>=0)
						parts[i].ctype = 0;
				}
				if ((r>>8)>=NPART || !r)
					continue;
				if ((r&0xFF)==PT_FIRE){
					sim->kill_part(r>>8);
						if(1>(rand()%150)){
							sim->kill_part(i);
							return 1;
						}
				}
				if (((r&0xFF)==PT_WATR || (r&0xFF)==PT_DSTW) && 1>(rand()%250))
				{
					sim->part_change_type(r>>8, x+rx, y+ry, PT_CBNW);
					if (parts[i].ctype==5) //conserve number of water particles - ctype=5 means this CO2 hasn't released the water particle from BUBW yet
						sim->create_part(i, x, y, PT_WATR);
					else
						sim->kill_part(i);
				}
			}
	if (parts[i].temp > 9773.15 && sim->pv[y/CELL][x/CELL] > 200.0f)
	{
		if (rand()%5 < 1)
		{
			int j;
			sim->create_part(i,x,y,PT_O2);

			j = sim->create_part(-3,x+rand()%3-1,y+rand()%3-1,PT_NEUT); if (j != -1) parts[j].temp = 15000;
			if (!(rand()%50)) { j = sim->create_part(-3,x+rand()%3-1,y+rand()%3-1,PT_ELEC); if (j != -1) parts[j].temp = 15000; }

			parts[i].temp = 15000;
			sim->pv[y/CELL][x/CELL] += 100;
		}
	}
	return 0;
}


Element_CO2::~Element_CO2() {}
