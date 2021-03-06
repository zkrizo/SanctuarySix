/*This header is designed to abstract the compiler/OS specific declaration types
**into a unified form to prevent memory issues.
*/
#ifndef UnitAbstraction_H
#define UnitAbstraction_H

#include <Windows.h>
#include <sstream>
#include <string>

#define OUTLAND_DEBUG 0

//redeclaration of types
typedef signed char		s8;
typedef unsigned char	u8;
typedef signed short	s16;
typedef unsigned short	u16;
typedef signed int		s32;
typedef unsigned int	u32;
typedef signed long		s64;
typedef unsigned long	u64;

//indifference types
typedef char	t8;
typedef short	t16;
typedef int		t32;
typedef long	t64;

typedef float			real32;
typedef double			real64;

typedef unsigned char	tBYTE;
typedef u32				tBOOL;

typedef size_t			tMEMSIZE;

//redeclarations of true and false to eliminate compiler bias
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


//Quaternion Tolerance Value
#define QUATERNION_TOLERANCE 0.000001

enum LoggerStates{
	LOG_ALL=1677215,
	LOG_MAINCORE=1,
	LOG_OPENGL=3,
	LOG_GRCORE=7,
	LOG_CHUNK=15,
	LOG_BLOCK=31,
	LOG_PLAYERCORE=63,
	LOG_INVENTORY=127,
	LOG_TECH=255,
	LOG_LOADER=511,
	LOG_GAMECORE=1023,
	LOG_BUILDING=2047,
	LOG_UICORE=4095,
	LOG_IO=8191,
	LOG_GUI=16383,
	LOG_AICORE=32767,
	LOG_BOT=65535,
	LOG_PATHFINDING=131071,
	LOG_POLYGON=262143,
	LOG_MESSAGER=524287,
	LOG_JOBQUEUE=1048575
};

enum GAMESTATE
{
	STATE_INTRO=0,
	STATE_LOADING,
	STATE_MAINMENU,
	STATE_GAME,
	STATE_INVENTORY,
	STATE_GAMEMENU,
	STATE_FIRSTPERSON,
	STATE_DEBUG=99,
};

enum UI
{
	UI_GAME=0,
	UI_RESOURCES,
	UI_BUILDING,
	UI_TECH,
	UI_MENU
};


enum BlockType
{
	BlockType_Default=0,

	BlockType_Air,
	BlockType_Water,
	BlockType_Lava,
	BlockType_Rock,
	BlockType_Dirt,
	BlockType_Sand,

	BlockType_MARKER,
	BlockType_numTypes,
};

enum ROOMTYPE
{
	RM_GHOST=-1,
	RM_HALLWAY=0,
	RM_AIRLOCK,
	RM_BATHROOM,
	RM_COMMANDCENTER,
	RM_FREIGHTAIRLOCK,
	RM_GALLEY,
	RM_INDUSTRIALWORKSHOP,
	RM_LABORATORY,
	RM_LIBRARY,
	RM_LIVINGQUARTERS,
	RM_MININGCONTROLCENTER,
	RM_OXYGENSCRUBBER,
	RM_POWERFUSION,
	RM_WAREHOUSE,
	RM_WATERCOLLECTOR,
	RM_WATERRECYCLING,

	RM_NUMTYPES
};

//enum for floor textures
enum Floortype
{
	FL_GRATE=0,
	FL_TILED,
	FL_METAL,

	FL_NUMFLOORS,
};

//enum for outer wall textures
enum OuterWall
{
	WALL_BASIC=0,

	WALL_NUMWALLS,
};

//Function Enum
enum FUNCTIONS{
	F_ATTACK=0,
	F_OPENINV,
	F_JUMP,
	F_MOVELEFT,
	F_MOVERIGHT,
	F_MOVEFORWARD,
	F_MOVEBACK,
	F_MENU,
	F_BUILDXP,
	F_BUILDXM,
	F_BUILDZP,
	F_BUILDZM,
	F_BUILDYP,
	F_BUILDYM,
	F_OPENTECH,
	F_OPENBUILD,
	F_ENTER,

	F_BLANK,
	F_NUMTYPES,
};

enum Resources
{
	RES_CURAIR=0,
	RES_MAXAIR,
	RES_CURPOWER,
	RES_MAXPOWER,
	RES_FOOD,
	RES_WATER,
	RES_COAL,
	RES_COPPER,
	RES_IRON,
	RES_GOLD,
	RES_PLATINUM,
	RES_QUARTZ,
	RES_SILICON,
	RES_URANIUM,
	RES_STEEL,
	RES_GLASS,
	RES_WIRING,

	RES_NUMRESOURCES,
};

enum DoorPlacement
{
	RM_DOOR0=0,
	RM_DOOR8,
	RM_DOOR86,
	RM_DOOR862,
	RM_DOOR864,
	RM_DOOR8624,
	RM_DOOR82,
	RM_DOOR824,
	RM_DOOR84,
	RM_DOOR6,
	RM_DOOR62,
	RM_DOOR624,
	RM_DOOR64,
	RM_DOOR2,
	RM_DOOR24,
	RM_DOOR4,
};

enum CornerPlacement
{
	RM_CORNER0=0,
	RM_CORNER9,
	RM_CORNER93,
	RM_CORNER931,
	RM_CORNER937,
	RM_CORNER9317,
	RM_CORNER91,
	RM_CORNER917,
	RM_CORNER97,
	RM_CORNER3,
	RM_CORNER31,
	RM_CORNER317,
	RM_CORNER37,
	RM_CORNER1,
	RM_CORNER17,
	RM_CORNER7,
};

enum TECH{
	TECH_NONE=1000,
	TECH_ELECTRONICS=0,
	TECH_SCIENCE,
	TECH_AGRICULTURE,
	TECH_ENERGY,

	NUM_TECHS
};

enum MODELS{
	MODEL_TEST=0,

	MODEL_TOTALNUM
};

enum GUIMenuTextures{	
	GUI_TITLE=0,
	GUI_NEWUNI,
	GUI_LOADUNI,
	GUI_MULTI,
	GUI_OPTIONS,
	GUI_MAINQUIT,

	GUI_MENUTOTAL
};

enum GUITextures{
	GUI_NONE=-1,
	GUI_MAIN=0,
	GUI_RESOURCES,
	GUI_BUILDING,
	GUI_TECH,
	GUI_VIEW,
	GUI_SYSTEM,
	GUI_WINDOW,
	GUI_SECONDWINDOW,
	GUI_CLOSE,
	GUI_QUIT,
	GUI_BUILD,
	GUI_DEMOLISH,
	GUI_SELECTICON,
	GUI_HALLWAYICON,
	GUI_BATHROOMICON,
	GUI_LIVINGICON,
	GUI_COMMANDICON,
	GUI_GALLEYICON,
	GUI_AIRLOCKICON,
	GUI_FREIGHTAIRLOCKICON,
	GUI_INDUSTRIALWORKSHOPICON,
	GUI_LABORATORYICON,
	GUI_LIBRARYICON,
	GUI_MCCICON,
	GUI_WAREHOUSEICON,
	GUI_POWERPLANTICON,
	GUI_DOORICON,
	GUI_SKIP,
	GUI_NEXT,
	GUI_TECH1TAB,
	GUI_TECH2TAB,
	GUI_TECH3TAB,
	GUI_TECH4TAB,
	GUI_OBJECTTAB,

	GUI_TOTALRESOURCES
};

enum FaceCodes{
	Face_YP=1,
	Face_YM=2,
	Face_XP=4,
	Face_XM=8,
	Face_ZP=16,
	Face_ZM=32,
	Face_ALL=63,
	Face_INVALID=64,
	Face_MERGE=128,
	Face_OUTLINE=256
};

enum CornerCodes{
	Corner9=1,
	Corner7=2,
	Corner1=4,
	Corner3=8
};
#endif