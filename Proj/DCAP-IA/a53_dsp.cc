#include "a53_dsp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const float dB_max=18.0f;    //闁灝鍘ら幑鐔锋綎閹殿剙锛愰崳顭掔礉闂勬劕鐣鹃張锟芥径褍顤冮惄濠冨付閸掞拷,閸楁洑缍呴弰顖氬瀻鐠愶拷

#define EQ2266
typedef struct _CoeFN2_
{
	CoefFloat beta1;
	CoefFloat alpha1;
	CoefFloat alpha2;
	_CoeFN2_(CoefFloat b1,CoefFloat a1,CoefFloat a2)
	{
		beta1=b1;
		alpha1=a1;
		alpha2=a2;
	}
}CoeFN2;

typedef struct
{
	CoeFN2 CoefN2[5];
}sIIRCoefficients;

static sIIRCoefficients eq_coeffs[] = 
{
	//BAND 1
	{
#if defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
		CoeFN2(3.012676261398012e-04, -1.999390645347651, 0.999397464747720),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#endif // #if defined(EQ2266)
	},
	//BAND 2
	{
#if defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
		CoeFN2(3.792438938622790e-04, -1.999230705037954, 0.999241512212275),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#endif // defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
	},
	//BAND 3
	{
#if defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
		CoeFN2(4.773929106353222e-04, -1.999028087652018, 0.999045214178729),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#endif // defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
	},
	//BAND 4
	{
#if defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
		CoeFN2(6.009278130675350e-04, -1.998771004034526, 0.998798144373865),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#endif // defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
	},
	//BAND 5
	{
#if defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
		CoeFN2(7.564056545466936e-04, -1.998444180897572, 0.998487188690907),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#endif // defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
	},
	//BAND 6
	{
#if defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
		CoeFN2(9.520719406408254e-04, -1.998027706842443, 0.998095856118718),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#endif // defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
	},
	//BAND 7
	{
#if defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.001198292374103, -1.997495432887108, 0.997603415251795),
		CoeFN2(1.000000000000000, -1.998549720195052, 0.998681660962238),
		CoeFN2(1.437625244907313e-06, -1.998831854032755, 0.998920334369594),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#endif // defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
	},
	//BAND 8
	{
#if defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.001508093229616, -1.996812726962698, 0.996983813540768),
		CoeFN2(1.000000000000000, -1.998131521416933, 0.998340596422706),
		CoeFN2(2.277775105983024e-06, -1.998500757113240, 0.998640968807033),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#endif // defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
	},
	//BAND 9
	{
#if defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
        CoeFN2(0.001897836658734, -1.995933280712867, 0.996204326682531),
		CoeFN2(1.000000000000000, -1.997580103724753, 0.997911390772596),
		CoeFN2(3.608619556249236e-06, -1.998067196816350, 0.998289376783647),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#endif // defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
	},
	//BAND 10
	{
#if defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.002388062987515, -1.994794511484789, 0.995223874024970),
		CoeFN2(1.000000000000000, -1.996846418213174, 0.997371322524008),
		CoeFN2(5.716463507155887e-06, -1.997494869295461, 0.997846919048107),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#endif // defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
	},
	//BAND 11
	{
#if defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.003004538731544, -1.993310862789098, 0.993990922536913),
		CoeFN2(1.000000000000000, -1.995860233383160, 0.996691846618003),
		CoeFN2(9.054375475011919e-06, -1.996732369474831, 0.997290165780113),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#endif // defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
	},
	//BAND 12
	{
#if defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.003779556520490, -1.991363936644139, 0.992440886959020),
		CoeFN2(1.000000000000000, -1.994519722536449, 0.995837123675968),
		CoeFN2(1.433903786178895e-05, -1.995705961137978, 0.996589674911891),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#endif // defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
	},
	//BAND 13
	{
#if defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.004753541112380, -1.988787820376789, 0.990492917775241),
		CoeFN2(1.000000000000000, -1.992675506980513, 0.994762189995608),
		CoeFN2(2.270356241122943e-05, -1.994308551238030, 0.995708465381306),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#endif // defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
	},
	//BAND 14
	{
#if defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.005977023563651, -1.985347091154665, 0.988045952872699),
		CoeFN2(1.000000000000000, -1.990106094831042, 0.993410691140535),
		CoeFN2(3.593833104183775e-05, -1.992382777302158, 0.994600109796311),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#endif // defined(EQ2266)||defined(EQ2666)||defined(EQ2668)||defined(EQ2688)
	},
	//BAND 15
	{
#if defined(EQ2266)||defined(EQ2666)
		CoeFN2(0.007513052270027, -1.980703632824276, 0.984973895459946),
		CoeFN2(1.000000000000000, -1.986479955704683, 0.991712090106528),
		CoeFN2(0.000056870011701, -1.989694950939902, 0.993206354768633),
		CoeFN2(0, 0, 0),
#elif defined(EQ2688)
		CoeFN2(5.697043409767465e-05, -1.988306716872045, 0.993613159123368),
		CoeFN2(1.000000000000000, -1.991365363264986, 0.994833709446418),
		CoeFN2(5.651010472357376e-05, -1.980831153763097, 0.985498972913121),
		CoeFN2(1.000000000000000, -1.982810123172665, 0.986721128818947),
#endif
		CoeFN2(0, 0, 0)
	},
	//BAND 16
	{
#if defined(EQ2266)||defined(EQ2666)
		CoeFN2(0.009440113054154, -1.974366346203043, 0.981119773891691),
		CoeFN2(1.000000000000000, -1.981296859305580, 0.989578254740512),
		CoeFN2(0.000089956921406, -1.985894790061920, 0.991454154937931),
		CoeFN2(0, 0, 0),
#elif defined(EQ2688)
		CoeFN2(9.015659699804740e-05, -1.983565835610836, 0.991966870032143),
		CoeFN2(1.000000000000000, -1.988007483301298, 0.993499841636092),
		CoeFN2(8.924278090639106e-05, -1.974396503377474, 0.981779021252597),
		CoeFN2(1.000000000000000, -1.977124088506002, 0.983310840472846),
#endif
		CoeFN2(0, 0, 0)
	},
	//BAND 17
	{
#if defined(EQ2266)||defined(EQ2666)
		CoeFN2(0.011855627623790, -1.965614744994898, 0.976288744752420),
		CoeFN2(1.000000000000000, -1.973797131865518, 0.986899333296771),
		CoeFN2(0.000142222047844, -1.980453086553249, 0.989251977457882),
		CoeFN2(0, 0, 0),
#elif defined(EQ2688)
		CoeFN2(1.426187171983776e-04, -1.976603466455665, 0.989898955576613),
		CoeFN2(1.000000000000000, -1.983127449449069, 0.991822607570016),
		CoeFN2(1.408070376024273e-04, -1.965447250467490, 0.977116173520379),
		CoeFN2(1.000000000000000, -1.969252067447765, 0.979033418878023),
#endif
		CoeFN2(0, 0, 0)
	},
	//BAND 18
	{
#if defined(EQ2266)||defined(EQ2666)
		CoeFN2(0.014880085790482, -1.953382872094262, 0.970239828419036),
		CoeFN2(1.000000000000000, -1.962821552730147, 0.983538853752743),
		CoeFN2(0.000224710916034, -1.972565218161340, 0.986485192885291),
		CoeFN2(0, 0, 0),
#elif defined(EQ2688)
		CoeFN2(2.254980342038570e-04, -1.966272428071592, 0.987303253265846),
		CoeFN2(1.000000000000000, -1.975953560751253, 0.989714050767649),
		CoeFN2(2.219121833338453e-04, -1.952848888356847, 0.971278137186209),
		CoeFN2(1.000000000000000, -1.958219878436494, 0.973673168816105),
#endif
		CoeFN2(0, 0, 0)
	},
	//BAND 19
	{
#if defined(EQ2266)||defined(EQ2666)
		CoeFN2(0.018661834950517, -1.936083459851454, 0.962676330098965),
		CoeFN2(1.000000000000000, -1.946596032884443, 0.979328054289964),
		CoeFN2(0.000354761024890, -1.961002092019685, 0.983010306776439),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#elif defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.000356320687380, -1.950805270447215, 0.984048158630700),
		CoeFN2(1.000000000000000, -1.965298506620184, 0.987063775146667),
		CoeFN2(0.000349237784910, -1.934904492300654, 0.963979378323813),
		CoeFN2(1.000000000000000, -1.942572527304247, 0.966963265518968),
		CoeFN2(0, 0, 0)
#else
		CoeFN2(0.018661834950517, -1.936083459851454, 0.962676330098965),
		CoeFN2(1.000000000000000, -1.953565707816584, 0.987064285019894),
		CoeFN2(3.573070728410000e-04, -1.967917343089768, 0.989576272830039),
		CoeFN2(1.000000000000000, -1.937154869777730, 0.967692600145966),
		CoeFN2(3.507174086754873e-04, -1.948391309123434, 0.971712048116558)
#endif
	},
	//BAND 20
	{
#if defined(EQ2266)||defined(EQ2666)
		CoeFN2(0.023382491255132, -1.911343467896403, 0.953235017489736),
		CoeFN2(1.000000000000000, -1.922403484686658, 0.974059613215399),
		CoeFN2(0.000559517908577, -1.943880986171392, 0.978647682751366),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#elif defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.000562602793338, -1.927478804255394, 0.979971515952051),
		CoeFN2(1.000000000000000, -1.949331866239335, 0.983733111658414),
		CoeFN2(0.000548648292830, -1.909067811644905, 0.954871349536735),
		CoeFN2(1.000000000000000, -1.920127511893625, 0.958574851823147),
		CoeFN2(0, 0, 0)
#else
		CoeFN2(0.023382491255132, -1.911343467896403, 0.953235017489736),
		CoeFN2(1.000000000000000, -1.930836252351360, 0.983752520024085),
		CoeFN2(5.645573277660722e-04, -1.952643652552862, 0.986888097702808),
		CoeFN2(1.000000000000000, -1.911383066661415, 0.959510261962556),
		CoeFN2(5.515517970789943e-04, -1.927719552530085, 0.964504237462468)
#endif
	},
	//BAND 21
	{
#if defined(EQ2266)||defined(EQ2666)
		CoeFN2(0.029262839464038, -1.875611535780564, 0.941474321071924),
		CoeFN2(1.000000000000000, -1.886090976521047, 0.967481277830112),
		CoeFN2(0.000881349860966, -1.918316370821191, 0.973172219707015),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#elif defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.000887437665955, -1.892107877633781, 0.974875327007813),
		CoeFN2(1.000000000000000, -1.925231821853341, 0.979547541985440),
		CoeFN2(0.000860033103646, -1.871517854297857, 0.943532511604368),
		CoeFN2(1.000000000000000, -1.887606207405624, 0.948104278186651),
		CoeFN2(0, 0, 0)
#else
		CoeFN2(0.029262839464038, -1.875611535780564, 0.941474321071924),
		CoeFN2(1.000000000000000, -1.896135290301963, 0.979609265207090),
		CoeFN2(8.913035457747309e-04, -1.929416715469342, 0.983507315084723),
		CoeFN2(1.000000000000000, -1.873615279538859, 0.949318905194281),
		CoeFN2(8.657069775604356e-04, -1.897561818623360, 0.955491547294149)
#endif
	},
	//BAND 22
	{
#if defined(EQ2266)||defined(EQ2666)
		CoeFN2(0.036568941605545, -1.823588425589284, 0.926862116788909),
		CoeFN2(1.000000000000000, -1.831347412360989, 0.959290551825823),
		CoeFN2(0.001386122892458, -1.879895983842752, 0.966301078561483),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#elif defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.001398102381851, -1.838299469503877, 0.968521173836958),
		CoeFN2(1.000000000000000, -1.888659666927097, 0.974286566317498),
		CoeFN2(0.001344498979276, -1.816543497088102, 0.929459441095835),
		CoeFN2(1.000000000000000, -1.840094015228469, 0.935058195730131),
		CoeFN2(0, 0, 0)
#else
		CoeFN2(0.036568941605545, -1.823588425589284, 0.926862116788909),
		CoeFN2(1.000000000000000, -1.843030071366806, 0.974438396075413),
		CoeFN2(0.001405731057210, -1.893937027530659, 0.979253713806106),
		CoeFN2(1.000000000000000, -1.817900185948327, 0.936664953588202),
		CoeFN2(0.001355529246274, -1.853207793772918, 0.944235979393561)
#endif
	},
	//BAND 23
	{
#if defined(EQ2266)||defined(EQ2666)
		CoeFN2(0.045617972506925, -1.747432679475713, 0.908764054986151),
		CoeFN2(1.000000000000000, -1.748689100489592, 0.949132903222330),
		CoeFN2(0.002175723846188, -1.821915227059952, 0.957676797550058),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#elif defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.002199211868967, -1.756398246683091, 0.960628325993218),
		CoeFN2(1.000000000000000, -1.832986437839262, 0.967669530296635),
		CoeFN2(0.002094884539740, -1.735690736464667, 0.912061454095996),
		CoeFN2(1.000000000000000, -1.770276773979537, 0.918835896194087),
		CoeFN2(0, 0, 0)
#else
		CoeFN2(0.045617972506925, -1.747432679475713, 0.908764054986151),
		CoeFN2(1.000000000000000, -1.761773143972067, 0.968008161621147),
		CoeFN2(0.002214221641180, -1.839612928784965, 0.973896968530061),
		CoeFN2(1.000000000000000, -1.735398668670812, 0.921018768176182),
		CoeFN2(0.002116190917132, -1.787604816959225, 0.930196948081797)
#endif
	},
	//BAND 24
	{
#if defined(EQ2266)||defined(EQ2666)
		CoeFN2(0.056783053515153, -1.635730842057019, 0.886433892969694),
		CoeFN2(1.000000000000000, -1.624144663315940, 0.936608556420120),
		CoeFN2(0.003406777892248, -1.734309813433243, 0.946842500416628),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#elif defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.003452621093350, -1.632107380190457, 0.950878699348846),
		CoeFN2(1.000000000000000, -1.748203769317497, 0.959334490861281),
		CoeFN2(0.003250821822818, -1.616672118987729, 0.890663174242332),
		CoeFN2(1.000000000000000, -1.667427149305386, 0.898707391579098),
		CoeFN2(0, 0, 0)
#else
		CoeFN2(0.056783053515153, -1.635730842057019, 0.886433892969694),
		CoeFN2(1.000000000000000, -1.637882162428659, 0.960054879441225),
		CoeFN2(0.003482043324856, -1.756456630664316, 0.967138499889185),
		CoeFN2(1.000000000000000, -1.613231327078814, 0.901781674567553),
		CoeFN2(0.003291654577298, -1.690335782269223, 0.912704865193328)
#endif
	},
	//BAND 25
	{
#if defined(EQ2266)||defined(EQ2666)
		CoeFN2(0.070496110864607, -1.472356069725432, 0.859007778270786),
		CoeFN2(1.000000000000000, -1.437827491073826, 0.921298330577861),
		CoeFN2(0.005318183088378, -1.602297763133348, 0.933202169356712),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#elif defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.005407145539574, -1.444968449772523, 0.938937478264505),
		CoeFN2(1.000000000000000, -1.619514441766061, 0.948803947115943),
		CoeFN2(0.005019777957039, -1.442198149216208, 0.864523187224267),
		CoeFN2(1.000000000000000, -1.516224725287344, 0.873783324827841),
		CoeFN2(0, 0, 0)
#else
		CoeFN2(0.070496110864607, -1.472356069725432, 0.859007778270786),
		CoeFN2(1.000000000000000, -1.450557356742400, 0.950299680906204),
		CoeFN2(0.005464544996704, -1.629641324334333, 0.958581882049685),
		CoeFN2(1.000000000000000, -1.433252415626024, 0.878313625812042),
		CoeFN2(0.005097280779972, -1.546390486291138, 0.890921532392603)
#endif
	},
	//BAND 26
	{
#if defined(EQ2266)||defined(EQ2666)
		CoeFN2(0.087247656376946, -1.235707322953386, 0.825504687246109),
		CoeFN2(1.000000000000000, -1.163125393063927, 0.902831061920498),
		CoeFN2(0.008270803107580, -1.404989832250032, 0.915949770117584),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#elif defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.008442195531043, -1.167446179378912, 0.924508724628079),
		CoeFN2(1.000000000000000, -1.425832270133421, 0.935423284259096),
		CoeFN2(0.007705574256597, -1.189317538406414, 0.832884682934320),
		CoeFN2(1.000000000000000, -1.295812803367929, 0.842966403018364),
		CoeFN2(0, 0, 0)
#else
		CoeFN2(0.087247656376946, -1.235707322953386, 0.825504687246109),
		CoeFN2(1.000000000000000, -1.171688100837045, 0.938494860544047),
		CoeFN2(0.008553496239522, -1.437931121646939, 0.947679498031349),
		CoeFN2(1.000000000000000, -1.171394375372814, 0.850003354362317),
		CoeFN2(0.007850964619792, -1.335081660869467, 0.863772193668220)
#endif
	},
	//BAND 27
	{
#if defined(EQ2266)||defined(EQ2666)
		CoeFN2(0.107582600981912, -0.899697059712080, 0.784834798036175),
		CoeFN2(1.000000000000000, -0.768518110723564, 0.881045808925629),
		CoeFN2(0.012803236669417, -1.114909476413851, 0.893925833436911),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#elif defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.013130437427286, -0.766714427634130, 0.907472030430792),
		CoeFN2(1.000000000000000, -1.139081653434087, 0.918235928481246),
		CoeFN2(0.011745696260371, -0.830842712242582, 0.795093538126609),
		CoeFN2(1.000000000000000, -0.980288018446989, 0.804855838450777),
		CoeFN2(0, 0, 0)
#else
		CoeFN2(0.107582600981912, -0.899697059712080, 0.784834798036175),
		CoeFN2(1.000000000000000, -0.767592385427157, 0.924539565047381),
		CoeFN2(0.013344583379390, -1.152843400209969, 0.933624196757689),
		CoeFN2(1.000000000000000, -0.799306738192875, 0.816431257857241),
		CoeFN2(0.012014616235727, -1.030208225087702, 0.829809383866957)
#endif
	},
	//BAND 28
	{
#if defined(EQ2266)||defined(EQ2666)
		CoeFN2(0.132092152661546, -0.439689478210567, 0.735815694676908),
		CoeFN2(1.000000000000000, -0.226638911995240, 0.856396560145309),
		CoeFN2(0.019707703077540, -0.701009690296659, 0.865279235122368),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#elif defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.020325232234957, -0.214032613324394, 0.888227899549108),
		CoeFN2(1.000000000000000, -0.726809030737928, 0.895689107590514),
		CoeFN2(0.017758280620902, -0.342429329061957, 0.750882486822046),
		CoeFN2(1.000000000000000, -0.543571341970792, 0.757516483506803),
		CoeFN2(0, 0, 0)
#else
		CoeFN2(0.132092152661546, -0.439689478210567, 0.735815694676908),
		CoeFN2(1.000000000000000, -0.208547262464873, 0.908775020922127),
		CoeFN2(0.020733173427185, -0.741009735559785, 0.915093905998900),
		CoeFN2(1.000000000000000, -0.292166484327404, 0.777765857899728),
		CoeFN2(0.018247390786563, -0.604283654181333, 0.786888406826521)
#endif
	},
	//BAND 29
	{
#if defined(EQ2266)||defined(EQ2666)
		CoeFN2(0.161404113314493, 0.150221327434957, 0.677191773371014),
		CoeFN2(1.000000000000000, -0.141326299955508, 0.826483702406485),
		CoeFN2(0.030129385214836, 0.460758691416618, 0.831132888381863),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#elif defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.031278226031959, -0.164160841276134, 0.864780419793677),
		CoeFN2(1.000000000000000, 0.488942831399455, 0.868706502615050),
		CoeFN2(0.026599401464121, 0.024999035077297, 0.697771492833906),
		CoeFN2(1.000000000000000, 0.277995921302308, 0.701199612420440),
		CoeFN2(0, 0, 0)
#else
		CoeFN2(0.161404113314493, 0.150221327434957 , 0.677191773371014),
		CoeFN2(1.000000000000000, -0.175731957770559,  0.889511236873969),
		CoeFN2(0.032045493661545, 0.504086743901484 , 0.892848770249405),
		CoeFN2(1.000000000000000, -0.043137553869180,  0.731200226250891),
		CoeFN2(0.027471573034715, 0.350193793411540 , 0.735925948038891)
#endif
	},
	//BAND 30
	{
#if defined(EQ2266)||defined(EQ2666)
		CoeFN2(0.196178319553366, 0.818786400393608, 0.607643360893268),
		CoeFN2(1.000000000000000, 0.541397239483108, 0.768314987955113),
		CoeFN2(0.045691066358089, 1.212053206644597, 0.814211051370613),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#elif defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.047790342076911, 0.531632609473428, 0.817593096399686),
		CoeFN2(1.000000000000000, 1.255579755726060, 0.856486416968323),
		CoeFN2(0.039432600831153, 0.682134611600780, 0.617995264438473),
		CoeFN2(1.000000000000000, 0.970663116668331, 0.651963664047646),
		CoeFN2(0, 0, 0)
#else
		CoeFN2(0.196178319553366, 0.818786400393608, 0.607643360893268),
		CoeFN2(1.000000000000000, 0.529295099093479, 0.850043908472292),
		CoeFN2(0.049210061146888, 1.280750112651828, 0.883210061297188),
		CoeFN2(1.000000000000000, 0.616019046009705, 0.653087994522558),
		CoeFN2(0.040952638753331, 1.061224921909832, 0.699736479957673)
#endif
	},
	//BAND 31
	{
#if defined(EQ2266)||defined(EQ2666)
		CoeFN2(0.237121815580611, 1.392504597639421, 0.525756368838778),
		CoeFN2(1.000000000000000, 1.172373646813100, 0.641131914727975),
		CoeFN2(0.068644141536955, 1.824575654152969, 0.870819878021659),
		CoeFN2(0, 0, 0),
		CoeFN2(0, 0, 0)
#elif defined(EQ2668)||defined(EQ2688)
		CoeFN2(0.072395495561185, 1.202686807356203, 0.712485258725576),
		CoeFN2(1.000000000000000, 1.858290652285853, 0.903135854959232),
		CoeFN2(0.057819071437291, 1.199895037762558, 0.457529716266622),
		CoeFN2(1.000000000000000, 1.612291827682061, 0.683163021791264),
		CoeFN2(0, 0, 0)
#else
		CoeFN2(0.237121815580611, 1.392504597639421, 0.525756368838778),
		CoeFN2(1.000000000000000, 1.226874080619417, 0.760970201721352),
		CoeFN2(0.074968090096617, 1.878012455601303, 0.922338602418284),
		CoeFN2(1.000000000000000, 1.154399162103221, 0.489126040065859),
		CoeFN2(0.060400149967139, 1.701841584845747, 0.758825251107051)
#endif
	},
};

inline float dBtoGainbyAmplitude(float db)      //婢规澘甯� 楠炲懎瀹�
{
	return pow(10,db/20.0f);
}

inline float DBtoGain(float db)
{
	return dBtoGainbyAmplitude(db);   //gain閺堬拷缂佸牐顩︽稊妯逛簰婢逛即鐓堕獮鍛閿涘本澧嶆禒銉洣閻€劌鐔�娴滃骸绠欐惔锔炬畱婢х偟娉潪顒佸床閸忣剙绱�

	//return dBtoGainbyPower(db);
	//娴犮儰绗呭▽鈩冩箒閺勫海鈥橀悶鍡氼啈娓氭繃宓侀敍灞界熬瀵拷 by lizulin
	//return 2.5220207857061455181125E-01 *
	//    exp(8.0178361802353992349168E-02 * db)
	//    -2.5220207852836562523180E-01;
	// return exp(8.0178361802353992349168E-02 * db);
}
inline void CycleCnt(unsigned int& current,unsigned int max)
{
	//閺傝纭�1:
	//current= (current+1)%max;   //閸欐牔缍戝▔锟�
	//閺傝纭�2
	unsigned int tmp=current+1;   //娑撳秶鏁ら梽銈嗙《閸欐牔缍戞潻鎰暬閿涘瞼绮″ù瀣纯韫囷拷
	//tmp=tmp+1;
	if (tmp>=max)
	{
		tmp=0;
	}
	current=tmp;
}

DSP gDsp;
DSP* GetDsp()
{
	return &gDsp;
}

DSP::DSP():bEQ(true)
{
	for(int n = 0; n < 256; n++)
		dither[n] = (rand() % 4) - 2;
	di = 0;

	LoadCoeff();
	InitZero();
}

void DSP::ClearHistoryAll()
{
	for(int nSpk = 0;nSpk < EQ_CHANNELS;nSpk++)
	{
		ClearHistorySpk(nSpk);
	}
}

void DSP::InitZero()
{
	ClearHistoryAll();
	//婢跺秳缍呴幍锟介張澶婎杻閻╁﹥甯堕崚璺哄棘閺侊拷
	for(int nSpk = 0; nSpk< EQ_CHANNELS; nSpk++)
	{
		SetSpkAmp(nSpk,0);        //婢跺秳缍呴幀璇差杻閻╋拷0dB
		ClearSpk(nSpk);              //婢跺秳缍呴崥鍕嚋妫版垶顔屾晶鐐垫抄
	}
}

DSP::~DSP()
{
}

void DSP::LoadCoeff()
{
	// 	memset(SSEeq_coeffs,0,sizeof(SSEeq_coeffs));   //韫囧懘銆忓〒鍛存祩閿涘苯鎯侀崚娆戭儑32娑撶寵rand閸欏倹鏆熸禒缁樺壈閸婄》绱濋崣顖濆厴閸戞椽鏁�
	int A,B;
	float *pf;
	for (int band = 0; band < 6; band++)
	{
		A = band / 4;
		B = band % 4;
		for(int j = 0; j < 2; j++) //2娑擃亙绨╅梼璺哄棘閺侊拷
		{
			pf = (float*)&SSEeq_coeffs[A].SSECoefN2[j].alpha1;
			pf[B] = eq_coeffs[band].CoefN2[j].alpha1;
			pf = (float*)&SSEeq_coeffs[A].SSECoefN2[j].alpha2;
			pf[B] = eq_coeffs[band].CoefN2[j].alpha2;
			pf = (float*)&SSEeq_coeffs[A].SSECoefN2[j].beta1;
			pf[B] = eq_coeffs[band].CoefN2[j].beta1;
		}
	}
	for (int i = 2; i < 4; i++)
	{
		for(int j = 0; j < 2; j++) //2娑擃亙绨╅梼璺哄棘閺侊拷
		{
			pf = (float*)&SSEeq_coeffs[A].SSECoefN2[j].alpha1;
			pf[i] = 0.0f;
			pf = (float*)&SSEeq_coeffs[A].SSECoefN2[j].alpha2;
			pf[i] = 0.0f;
			pf = (float*)&SSEeq_coeffs[A].SSECoefN2[j].beta1;
			pf[i] = 0.0f;
		}
	}

	for (int band = 6; band < 14; band++)
	{
		A = (band+2) / 4;
		B = (band+2) % 4;
		for(int j = 0; j < 3; j++) //3娑擃亙绨╅梼璺哄棘閺侊拷
		{
			pf = (float*)&SSEeq_coeffs[A].SSECoefN2[j].alpha1;
			pf[B] = eq_coeffs[band].CoefN2[j].alpha1;
			pf = (float*)&SSEeq_coeffs[A].SSECoefN2[j].alpha2;
			pf[B] = eq_coeffs[band].CoefN2[j].alpha2;
			pf = (float*)&SSEeq_coeffs[A].SSECoefN2[j].beta1;
			pf[B] = eq_coeffs[band].CoefN2[j].beta1;
		}
	}
	for (int band = 14; band < 18; band++)
	{
		A = (band+2) / 4;
		B = (band+2) % 4;
		for(int j = 0; j < 4; j++) //4娑擃亙绨╅梼璺哄棘閺侊拷
		{
			pf = (float*)&SSEeq_coeffs[A].SSECoefN2[j].alpha1;
			pf[B] = eq_coeffs[band].CoefN2[j].alpha1;
			pf = (float*)&SSEeq_coeffs[A].SSECoefN2[j].alpha2;
			pf[B] = eq_coeffs[band].CoefN2[j].alpha2;
			pf = (float*)&SSEeq_coeffs[A].SSECoefN2[j].beta1;
			pf[B] = eq_coeffs[band].CoefN2[j].beta1;
		}
	}
	for (int band = 18; band < EQ_BANDS; band++)
	{
		A = (band+2) / 4;
		B = (band+2) % 4;
		for(int j = 0; j < 5; j++) //5娑擃亙绨╅梼璺哄棘閺侊拷
		{
			pf = (float*)&SSEeq_coeffs[A].SSECoefN2[j].alpha1;
			pf[B] = eq_coeffs[band].CoefN2[j].alpha1;
			pf = (float*)&SSEeq_coeffs[A].SSECoefN2[j].alpha2;
			pf[B] = eq_coeffs[band].CoefN2[j].alpha2;
			pf = (float*)&SSEeq_coeffs[A].SSECoefN2[j].beta1;
			pf[B] = eq_coeffs[band].CoefN2[j].beta1;
		}
	}
}

void DSP::EqSpkNSample(int *out, int *in, unsigned int nSpk,unsigned int nSample,bool hasSound)
{
    int* InBuf = in;
    int* OutBuf = out;

    float tmpOut;  //old double閺�绛琹oat 0928
    float nSpkOutGain=SpkOutGain[nSpk];   //閺�鎯у煂閸撳秹娼伴弴鏉戞彥
    float resultTmp[4];
    for(unsigned int index = 0; index < nSample; index++)
	{
// 		pcm = InBuf[index];//*SpkAmp[nSpk];   //tmpIn[index];   //閹顤冮惄濠冨付閸掕泛绨茬拠銉︽杹閸︺劏绶崙锟�*gain 閺囨潙鎮庨悶锟� 閺堫剝宸濇稉锟介弽椋庢畱

		float32x4_t pcmin=vdupq_n_f32(InBuf[index] + dither[di]);
		float32x4_t ret;
		float32x4_t acc=vdupq_n_f32(0);
		float32x4_t beta1;
		float32x4_t alpha1;
		float32x4_t alpha2;
		float32x4_t x2;
		float32x4_t y1, y2;
		float32x4_t gain;

	//
#if 1
		//BAND 0-5 閸嬶拷4闂冿拷
		for(int band = 0; band < 2; band++)
		{
			SSECoefficients& cof=SSEeq_coeffs[band];
			sXYData& sData=data_historySpk[band][nSpk];

			ret = pcmin;	    //濞夈劍鍓拌箛鍛淬�忛崷銊︻劃婢跺嫰鍣搁弬鐗堝复閸欐绶崗顧禼m
			int N = 0;
//			for (int N=0;N<1;N++)   //4娑擄拷2闂冨墎楠囬懕锟�
			{
				beta1 = cof.SSECoefN2[N].beta1;
				alpha1 = cof.SSECoefN2[N].alpha1;
				alpha2 = cof.SSECoefN2[N].alpha2;

				N2& mN2=iNSpk[nSpk].iN2[N];
				vst1q_f32(&sData.sXYN2[N].x[mN2.in[0]][0], ret);
				x2 = vld1q_f32(&sData.sXYN2[N].x[mN2.in[2]][0]);
				y1 = vld1q_f32(&sData.sXYN2[N].y[mN2.in[1]][0]);
				y2 = vld1q_f32(&sData.sXYN2[N].y[mN2.in[2]][0]);

//				ret = vmlsq_f32(vmlsq_f32(vmulq_f32(beta1, vsubq_f32(ret, x2)), alpha1, y1), alpha2, y2);

				ret = vsubq_f32(vsubq_f32(vmulq_f32(vsubq_f32(ret, x2), beta1), vmulq_f32(alpha1, y1)), vmulq_f32(alpha2, y2));
//				ret = vfmsq_f32(vfmsq_f32(vmulq_f32(beta1, vsubq_f32(ret, x2)), alpha1, y1), alpha2, y2);

//				ret = vmulq_f32(beta1, ret);
//				ret = vmlsq_f32(ret, beta1, x2);
//				ret = vmlsq_f32(ret, alpha1, y1);
//				ret = vmlsq_f32(ret, alpha2, y2);

				vst1q_f32(&sData.sXYN2[N].y[mN2.in[0]][0], ret);
			}
#if 1
			if(band == 1)
			{
				vst1q_f32(resultTmp, ret);
				resultTmp[2] = resultTmp[3] = 0.f;
				ret = vld1q_f32(resultTmp);
			}
#endif
			// 閺�鍦暏閸楁洘瀵氭禒銈呯杽閻滐拷 [1/3/2019 jaontolt]
			gain = vld1q_f32(&SSE_SpkGain[band][nSpk][0]);
//			acc = vmlaq_f32(acc, ret, gain);

			acc = vaddq_f32(vmulq_f32(ret, gain), acc);
//			acc = vfmaq_f32(acc, ret, gain);
		}
#endif // 0
#if 1
		//BAND 6-13 閸嬶拷6闂冿拷
		for(int band = 2; band < 4; band++)
		{
			SSECoefficients& cof=SSEeq_coeffs[band];
			sXYData& sData=data_historySpk[band][nSpk];

			ret = pcmin;	    //濞夈劍鍓拌箛鍛淬�忛崷銊︻劃婢跺嫰鍣搁弬鐗堝复閸欐绶崗顧禼m
#if defined(EQ2266)
			int N = 0;
#else
			for (int N=0;N<3;N++)   //4娑擄拷2闂冨墎楠囬懕锟�
#endif
			{
				beta1 = cof.SSECoefN2[N].beta1;
				alpha1 = cof.SSECoefN2[N].alpha1;
				alpha2 = cof.SSECoefN2[N].alpha2;

				N2& mN2=iNSpk[nSpk].iN2[N];
				vst1q_f32(&sData.sXYN2[N].x[mN2.in[0]][0], ret);
				x2 = vld1q_f32(&sData.sXYN2[N].x[mN2.in[2]][0]);
				y1 = vld1q_f32(&sData.sXYN2[N].y[mN2.in[1]][0]);
				y2 = vld1q_f32(&sData.sXYN2[N].y[mN2.in[2]][0]);

//				ret = vmlsq_f32(vmlsq_f32(vmulq_f32(beta1, vsubq_f32(ret, x2)), alpha1, y1), alpha2, y2);

//				ret = vfmsq_f32(vfmsq_f32(vmulq_f32(beta1, vsubq_f32(ret, x2)), alpha1, y1), alpha2, y2);
				ret = vsubq_f32(vsubq_f32(vmulq_f32(vsubq_f32(ret, x2), beta1), vmulq_f32(alpha1, y1)), vmulq_f32(alpha2, y2));

//				ret = vmulq_f32(beta1, pcmtmp);
//				ret = vmlsq_f32(ret, beta1, x2);
//				ret = vmlsq_f32(ret, alpha1, y1);
//				ret = vmlsq_f32(ret, alpha2, y2);

				vst1q_f32(&sData.sXYN2[N].y[mN2.in[0]][0], ret);
			}

			// 閺�鍦暏閸楁洘瀵氭禒銈呯杽閻滐拷 [1/3/2019 jaontolt]
			gain = vld1q_f32(&SSE_SpkGain[band][nSpk][0]);
//			acc = vmlaq_f32(acc, ret, gain);

			acc = vaddq_f32(vmulq_f32(ret, gain), acc);
//			acc = vfmaq_f32(acc, ret, gain);
		}
#endif

#if 1
		//BAND 14-17 閸嬶拷8闂冿拷
		{
			SSECoefficients& cof=SSEeq_coeffs[4];
			sXYData& sData=data_historySpk[4][nSpk];

			ret = pcmin;	    //濞夈劍鍓拌箛鍛淬�忛崷銊︻劃婢跺嫰鍣搁弬鐗堝复閸欐绶崗顧禼m
#if defined(EQ2266)||defined(EQ2666)||defined(EQ2668)
			for (int N = 0; N < 3; N++)
#else
			for (int N = 0; N < 4; N++)
#endif
			{
				beta1 = cof.SSECoefN2[N].beta1;
				alpha1 = cof.SSECoefN2[N].alpha1;
				alpha2 = cof.SSECoefN2[N].alpha2;

				N2& mN2=iNSpk[nSpk].iN2[N];
				vst1q_f32(&sData.sXYN2[N].x[mN2.in[0]][0], ret);

				x2 = vld1q_f32(&sData.sXYN2[N].x[mN2.in[2]][0]);
				y1 = vld1q_f32(&sData.sXYN2[N].y[mN2.in[1]][0]);
				y2 = vld1q_f32(&sData.sXYN2[N].y[mN2.in[2]][0]);

//				ret = vmlsq_f32(vmlsq_f32(vmulq_f32(beta1, vsubq_f32(ret, x2)), alpha1, y1), alpha2, y2);

//				ret = vfmsq_f32(vfmsq_f32(vmulq_f32(beta1, vsubq_f32(ret, x2)), alpha1, y1), alpha2, y2);
				ret = vsubq_f32(vsubq_f32(vmulq_f32(vsubq_f32(ret, x2), beta1), vmulq_f32(alpha1, y1)), vmulq_f32(alpha2, y2));

//				ret = vmulq_f32(beta1, pcmtmp);
//				ret = vmlsq_f32(ret, beta1, x2);
//				ret = vmlsq_f32(ret, alpha1, y1);
//				ret = vmlsq_f32(ret, alpha2, y2);

				vst1q_f32(&sData.sXYN2[N].y[mN2.in[0]][0], ret);
			}

			// 閺�鍦暏閸楁洘瀵氭禒銈呯杽閻滐拷 [1/3/2019 jaontolt]
			gain = vld1q_f32(&SSE_SpkGain[4][nSpk][0]);
//			acc = vmlaq_f32(acc, ret, gain);

			acc = vaddq_f32(vmulq_f32(ret, gain), acc);
//			acc = vfmaq_f32(acc, ret, gain);
		}
#endif // 0

#if 1
		//BAND 18-30 閸嬶拷10闂冿拷
		for(int band = 5; band < 9; band++)
		{
			SSECoefficients& cof=SSEeq_coeffs[band];
			sXYData& sData=data_historySpk[band][nSpk];

			ret = pcmin;	    //濞夈劍鍓拌箛鍛淬�忛崷銊︻劃婢跺嫰鍣搁弬鐗堝复閸欐绶崗顧禼m

#if defined(EQ2266)||defined(EQ2666)
			for (int N = 0; N < 3; N++)
#elif defined(EQ2668)||(EQ2688)
			for (int N = 0; N < 4; N++)
#else
			for (int N = 0; N < 5; N++)
#endif
			{
				beta1 = cof.SSECoefN2[N].beta1;
				alpha1 = cof.SSECoefN2[N].alpha1;
				alpha2 = cof.SSECoefN2[N].alpha2;

				N2& mN2=iNSpk[nSpk].iN2[N];
				vst1q_f32(&sData.sXYN2[N].x[mN2.in[0]][0], ret);
				x2 = vld1q_f32(&sData.sXYN2[N].x[mN2.in[2]][0]);
				y1 = vld1q_f32(&sData.sXYN2[N].y[mN2.in[1]][0]);
				y2 = vld1q_f32(&sData.sXYN2[N].y[mN2.in[2]][0]);
//				ret = vmlsq_f32(vmlsq_f32(vmulq_f32(beta1, vsubq_f32(ret, x2)), alpha1, y1), alpha2, y2);

//				ret = vfmsq_f32(vfmsq_f32(vmulq_f32(beta1, vsubq_f32(ret, x2)), alpha1, y1), alpha2, y2);
				ret = vsubq_f32(vsubq_f32(vmulq_f32(vsubq_f32(ret, x2), beta1), vmulq_f32(alpha1, y1)), vmulq_f32(alpha2, y2));

//				ret = vmulq_f32(beta1, pcmtmp);
//				ret = vmlsq_f32(ret, beta1, x2);
//				ret = vmlsq_f32(ret, alpha1, y1);
//				ret = vmlsq_f32(ret, alpha2, y2);

				vst1q_f32(&sData.sXYN2[N].y[mN2.in[0]][0], ret);
			}
#if 1
			if(band == 8)
			{
				vst1q_f32(resultTmp, ret);
				resultTmp[1] = resultTmp[2] = resultTmp[3] = 0.f;
				ret = vld1q_f32(resultTmp);
// 				ret.m256_f32[5] = ret.m256_f32[6] = ret.m256_f32[7] = 0;
			}
#endif

			// 閺�鍦暏閸楁洘瀵氭禒銈呯杽閻滐拷 [1/3/2019 jaontolt]
			gain = vld1q_f32(&SSE_SpkGain[band][nSpk][0]);
//			acc = vmlaq_f32(acc, ret, gain);

			acc = vaddq_f32(vmulq_f32(ret, gain), acc);
//			acc = vfmaq_f32(acc, ret, gain);
		}
#endif
		vst1q_f32(resultTmp, acc);
		tmpOut=resultTmp[0]+resultTmp[1]+resultTmp[2]+resultTmp[3];

		//NOTE: The IIR equation is *	y[n] = 2 * (alpha*(x[n]-x[n-2]) + gamma*y[n-1] - beta*y[n-2])*
		//Now the 2 factor has been distributed in the coefficients
		//鏉堟挸鍤惔鏃囶嚉*2 閼板奔绗夐弰锟�*3.5 閺�鎯у煂SpkAmp闁诧拷
		//閹顤冮惄濠冨付閸掕埖鏂侀崚鎷岀翻閸戝搫顦�

		//閸橈拷*3.5閺�鎯у煂SpkAmp闁插矂娼伴敍灞剧槨娑擃亪鍣伴弽椋庡仯閼哄倻瀹虫稉锟藉▎鈥茬濞夛拷 閸欘垯浜掓潻娑楃濮濄儱甯囩紓鈺佸煂*gain[brand]婢讹拷 閺堫剝宸濇稉锟介弽椋庢畱
		OutBuf[index] =(tmpOut - dither[di]) * nSpkOutGain;//LimitRange(tmpint);  //Int24 閻ㄥ埣paretor=(int) 瀹歌尙绮￠崑姘崇Ш閻ｅ苯鍨介弬顓濈啊閿涘本鐥呴張澶婄箑鐟曚礁婀銈夊櫢婢讹拷 by lizulin 20180928
		di = (di + 1) % 256;

		//鏉堟挸鍙嗘潏鎾冲毉娴ｅ秶鐤嗙拋鈩冩殶閸ｃ劌鎯婇悳顖濐吀閺侊拷
		for(int N = 0; N < 5; N++)
		{
			for (int i = 0; i < 3; i++)
			{
				CycleCnt(iNSpk[nSpk].iN2[N].in[i], 3);
			}
		}
	}
}

void DSP::SetSpkGain(unsigned int nSpk, unsigned int nBand, float db)
{

	if (nSpk>=(EQ_CHANNELS)||nBand>=EQ_BANDS)
		return;

	if (db>dB_max)
	{
		db=dB_max;
	}

	SpkGain[nBand][nSpk] = DBtoGain(db);   //1.0
	float gain=SpkGain[nBand][nSpk];
	//#endif // BAND61

	int A, B;
	if(nBand >= 0 && nBand <= 5)
	{
		A = nBand / 4;
		B = nBand % 4;
	}
	else if(nBand >=6 && nBand <= 13)
	{
		A = (nBand+2) / 4;
		B = (nBand+2) % 4;
	}
	else if(nBand >= 14 && nBand <= 17)
	{
		A = (nBand+2) / 4;
		B = (nBand+2) % 4;
	}
	else if(nBand >=18 && nBand <= 30)
	{
		A = (nBand+2) / 4;
		B = (nBand+2) % 4;
	}

	float* pf = (float*)&SSE_SpkGain[A][nSpk][0];
	pf[B] = gain;  //float
	//             printf("SetSpkGain %d band %d %d %f\n", nSpk, newBand/8, newBand%8, gain);


	if(SpkAbsGain[nBand][nSpk]==db)  //閻╃鎮撳▽鈩冩箒韫囧懓顩︾拋鎯х暰閿涘矂浼╅崗宥夘暥缁讳礁鐫嗛獮鏇熷ⅵ閸楋拷
	{
		return;
	}
	SpkAbsGain[nBand][nSpk] =db;


	if (nSpk>=EQ_CHANNELS)  //bed 闂冮潧鍨�
	{
		//		printf("SetSpkGain() nBed:%d nBand:%d db:%f gain:%f\n",nSpk-EQ_CHANNELS, nBand,db,DBtoGain(db));
	}
	else
	{
		//		printf("SetSpkGain() nSpk:%d nBand:%d db:%f gain:%f\n",nSpk, nBand,db,DBtoGain(db));
	}
}


void DSP::SetSpkGain(unsigned int nSpk, float gain[EQ_BANDS])
{
	for(int nband = 0; nband < EQ_BANDS; nband++)
	{
		SetSpkGain(nSpk,nband,gain[nband]);
	}
}

void DSP::SetSpkAmp(unsigned int nSpk, float db)
{
    if (nSpk>=EQ_CHANNELS)
        return;

    if (db>dB_max)
    {
        db=dB_max;
    }

    SpkAmp[nSpk] = DBtoGain(db);
    //IIR鏉堟挸鍤�*2閺�鎯у煂濮濄倕顦�  //閸樼喎娼庣悰鈩冪槨娑擃亪鍣伴弽椋庡仯鏉堟挸鍤紒鎾寸亯*3.5 缁夎鍩屽銈咁槱,*3.5闂堢偛绻�鐟曪拷
    //SpkOutGain[nSpk]= SpkAmp[nSpk]*2.0;
    SpkOutGain[nSpk]= SpkAmp[nSpk];//*3.0;			//GUANG AN MEN濞ｅ嘲鎮庡銈嗗皾閸ｃ劍鐥呴張澶岀埠娑擄拷閻ㄥ嫬顤冮惄濠吽夐崑鍖＄礉閺勵垰鍨庡▓鐢垫畱閿涘本澧嶆禒銉︽杹閸掔檴Q娑擃叀藟閸嬶拷


    //閸婅壈顔曠純顔煎閸氬孩鐥呴張澶嬫暭閸欙拷,闁灝鍘ゆ０鎴犵畳鐏炲繐绠烽幍鎾冲祪
    if(SpkAbsAmp[nSpk]==db)
    {
        return;
    }
    SpkAbsAmp[nSpk] = db;

    if (nSpk>=EQ_CHANNELS)  //bed 闂冮潧鍨�
    {
//        printf("DSP::SetSpkAmp() nBed:%d db:%f gain:%f\n",nSpk-EQ_CHANNELS,db,DBtoGain(db));
    }
    else
    {
//        printf("DSP::SetSpkAmp() nSpk:%d db:%f gain:%f\n",nSpk,db,DBtoGain(db));
    }
}


int DSP::ClearSpk(unsigned int nSpk)
{
	if (nSpk>=EQ_CHANNELS)
		return 0;
	float gain0dB= DBtoGain(0);

	for(int i = 0; i < EQ_BANDS; i++)
	{
		SpkGain[i][nSpk] = gain0dB;
		SpkAbsGain[i][nSpk] = 0;
	}

	//韫囶偊锟界喓鐣诲▔锟�
	for (int i = 0; i < MAX_SSE_VECTORS; i++)
	{
		for(int j = 0; j < 4; j++)
			SSE_SpkGain[i][nSpk][j] = gain0dB;
	}
	return 0;   //
}

void DSP::ClearHistorySpk(unsigned int nSpk)
{
	if (nSpk>=EQ_CHANNELS)
		return;

	//濞撳懘娅庨弻鎰嚋婢逛即浜� sXYData閻ㄥ嫬宸婚崣鍙夋殶閹癸拷
	for(int nband = 0; nband< EQ_BANDS;nband++)
	{
		//memset(&data_historySpk[nband][nSpk],0,sizeof(data_historySpk[nband][nSpk]));   //闁氨鏁ら崘娆愮《
		memset(&data_historySpk[nband][nSpk],0,sizeof(data_historySpk[0][0]));  //20181018
	}

	//閹垹顦茬粔璁崇秴鐠佲剝鏆熼崳锟�
	for(int N = 0; N < 5; N++)
	{
		for (int i = 0; i < 3; i++)
		{
			iNSpk[nSpk].iN2[N].in[i] = 3 - i - 1;
		}
	}
}

void DSP::SetAllSpkGain(float gain[EQ_BANDS][EQ_CHANNELS])
{
	for(int nSpk = 0; nSpk < EQ_CHANNELS; nSpk++)
	{
		for(int nband = 0; nband< EQ_BANDS; nband++)
		{
			SetSpkGain(nSpk,nband,gain[ nband][nSpk]);
		}
	}
}

void DSP::SetAllSpkAmp(float db[EQ_CHANNELS])
{
	for(int nSpk = 0; nSpk < EQ_CHANNELS; nSpk++)
	{
		SetSpkAmp(nSpk,db[nSpk]);
	}
}

void DSP::printParam()
{
	for(int i = 0; i < 31; i++)
	{
		printf("{");
		for (int j = 0; j < 31; j++)
		{
			printf("%ff, ", SpkGain[i][j]);
		}
		printf("},\n");
	}
	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n{");
	for(int i = 0; i < 31; i++)
	{
		printf("%ff, ", SpkAmp[i]);
	}
	printf("},\n");
}



