#ifndef WINAPI
	#define WINAPI
#endif

#ifdef __cplusplus
extern "C" {
#endif
	SHORT WINAPI mdOpen( SHORT, SHORT, LPLONG );
	SHORT WINAPI mdClose( LONG );
	SHORT WINAPI mdSend( LONG, SHORT, SHORT, SHORT, PSHORT, LPVOID );
	SHORT WINAPI mdReceive( LONG, SHORT, SHORT, SHORT, PSHORT, LPVOID );
	SHORT WINAPI mdDevSet( LONG, SHORT, SHORT, SHORT );
	SHORT WINAPI mdDevRst( LONG, SHORT, SHORT, SHORT );
	SHORT WINAPI mdRandW( LONG, SHORT, LPVOID, LPVOID, SHORT );
	SHORT WINAPI mdRandR( LONG, SHORT, LPVOID, LPVOID, SHORT );
	SHORT WINAPI mdControl( LONG, SHORT, SHORT );
	SHORT WINAPI mdTypeRead( LONG, SHORT, PSHORT );
	SHORT WINAPI mdBdLedRead( LONG, PSHORT );
	SHORT WINAPI mdBdModRead( LONG, PSHORT );
	SHORT WINAPI mdBdModSet( LONG, SHORT );
	SHORT WINAPI mdBdRst( LONG );
	SHORT WINAPI mdBdSwRead( LONG, PSHORT );
	SHORT WINAPI mdBdVerRead( LONG, PSHORT );
	SHORT WINAPI mdInit( LONG );
	SHORT WINAPI mdWaitBdEvent( LONG, PSHORT, LONG, PSHORT, PSHORT );
	LONG WINAPI mdSendEx( LONG, LONG, LONG, LONG, LONG, LPLONG, LPVOID );
	LONG WINAPI mdReceiveEx( LONG, LONG, LONG, LONG, LONG, LPLONG, LPVOID );
	LONG WINAPI mdDevSetEx( LONG, LONG, LONG, LONG, LONG );
	LONG WINAPI mdDevRstEx( LONG, LONG, LONG, LONG, LONG );
	LONG WINAPI mdRandWEx( LONG, LONG, LONG, LPVOID, LPVOID, LONG );
	LONG WINAPI mdRandREx( LONG, LONG, LONG, LPVOID, LPVOID, LONG );
	LONG WINAPI mdRemBufWriteEx(  LONG, LONG, LONG, LONG, LPLONG, LPVOID );
	LONG WINAPI mdRemBufReadEx(  LONG, LONG, LONG, LONG, LPLONG, LPVOID );
#ifdef __cplusplus
}
#endif

#define	mdopen			mdOpen
#define	mdclose			mdClose
#define	mdsend			mdSend
#define	mdreceive		mdReceive
#define	mddevset		mdDevSet
#define	mddevrst		mdDevRst
#define	mdrandw			mdRandW
#define	mdrandr			mdRandR
#define	mdcontrol		mdControl
#define	mdtyperead		mdTypeRead
#define	mdsendex		mdSendEx
#define	mdreceiveex		mdReceiveEx
#define	mddevsetex		mdDevSetEx
#define	mddevrstex		mdDevRstEx
#define	mdrandwex		mdRandWEx
#define	mdrandrex		mdRandREx

#define	DevX		(1)
#define	DevLX(x)	(DevX*1000+(x))
#define	DevY		(2)
#define	DevLY(x)	(DevY*1000+(x))
#define	DevL		(3)
#define	DevM		(4)
#define	DevSM		(5)
#define	DevF		(6)
#define	DevTT		(7)
#define	DevTC		(8)
#define	DevCT		(9)
#define	DevCC		(10)
#define	DevTN		(11)
#define	DevCN		(12)
#define	DevD		(13)
#define	DevSD		(14)
#define	DevTM		(15)
#define	DevTS		(16)
#define	DevTS2		(16002)
#define	DevTS3		(16003)
#define	DevCM		(17)
#define	DevCS		(18)
#define	DevCS2		(18002)
#define	DevCS3		(18003)
#define	DevA		(19)
#define	DevZ		(20)
#define	DevV		(21)
#define	DevR		(22)
#define	DevZR		(220)
#define	DevB		(23)
#define	DevLB(x)	(DevB*1000+(x))
#define	DevW		(24)
#define	DevLW(x)	(DevW*1000+(x))
#define	DevQSB		(25)
#define	DevLSB(x)	(DevQSB*1000+(x))
#define	DevSTT		(26)
#define	DevSTC		(27)
#define	DevQSW		(28)
#define	DevLSW(x)	(DevQSW*1000+(x))
#define	DevSPG(x)	(29*1000+(x))
#define	DevQV		(30)
#define	DevMRB		(33)
#define	DevMAB		(34)
#define	DevSTN		(35)
#define	DevWw		(36)
#define	DevWr		(37)
#define	DevLZ		(38)
#define	DevRD		(39)
#define DevFS		(40)
#define	DevLTT		(41)
#define	DevLTC		(42)
#define	DevLTN		(43)
#define	DevLCT		(44)
#define	DevLCC		(45)
#define	DevLCN		(46)
#define	DevLSTT		(47)
#define	DevLSTC		(48)
#define	DevLSTN		(49)
#define	DevSPB		(50)
#define	DevSPB1		(501)
#define	DevSPB2		(502)
#define	DevSPB3		(503)
#define	DevSPB4		(504)
#define	DevSPX		(51)
#define	DevSPY		(52)
#define	DevUSER		(100)
#define	DevMAIL		(101)
#define	DevMAILNC	(102)
#define	DevER0		(22000)
#define	DevER1		(22001)
#define	DevER2		(22002)
#define	DevER3		(22003)
#define	DevER4		(22004)
#define	DevER5		(22005)
#define	DevER6		(22006)
#define	DevER7		(22007)
#define	DevER8		(22008)
#define	DevER9		(22009)
#define	DevER10		(22010)
#define	DevER11		(22011)
#define	DevER12		(22012)
#define	DevER13		(22013)
#define	DevER14		(22014)
#define	DevER15		(22015)
#define	DevER16		(22016)
#define	DevER17		(22017)
#define	DevER18		(22018)
#define	DevER19		(22019)
#define	DevER20		(22020)
#define	DevER21		(22021)
#define	DevER22		(22022)
#define	DevER23		(22023)
#define	DevER24		(22024)
#define	DevER25		(22025)
#define	DevER26		(22026)
#define	DevER27		(22027)
#define	DevER28		(22028)
#define	DevER29		(22029)
#define	DevER30		(22030)
#define	DevER31		(22031)
#define	DevER32		(22032)
#define	DevER33		(22033)
#define	DevER34		(22034)
#define	DevER35		(22035)
#define	DevER36		(22036)
#define	DevER37		(22037)
#define	DevER38		(22038)
#define	DevER39		(22039)
#define	DevER40		(22040)
#define	DevER41		(22041)
#define	DevER42		(22042)
#define	DevER43		(22043)
#define	DevER44		(22044)
#define	DevER45		(22045)
#define	DevER46		(22046)
#define	DevER47		(22047)
#define	DevER48		(22048)
#define	DevER49		(22049)
#define	DevER50		(22050)
#define	DevER51		(22051)
#define	DevER52		(22052)
#define	DevER53		(22053)
#define	DevER54		(22054)
#define	DevER55		(22055)
#define	DevER56		(22056)
#define	DevER57		(22057)
#define	DevER58		(22058)
#define	DevER59		(22059)
#define	DevER60		(22060)
#define	DevER61		(22061)
#define	DevER62		(22062)
#define	DevER63		(22063)
#define	DevER64		(22064)
#define	DevER65		(22065)
#define	DevER66		(22066)
#define	DevER67		(22067)
#define	DevER68		(22068)
#define	DevER69		(22069)
#define	DevER70		(22070)
#define	DevER71		(22071)
#define	DevER72		(22072)
#define	DevER73		(22073)
#define	DevER74		(22074)
#define	DevER75		(22075)
#define	DevER76		(22076)
#define	DevER77		(22077)
#define	DevER78		(22078)
#define	DevER79		(22079)
#define	DevER80		(22080)
#define	DevER81		(22081)
#define	DevER82		(22082)
#define	DevER83		(22083)
#define	DevER84		(22084)
#define	DevER85		(22085)
#define	DevER86		(22086)
#define	DevER87		(22087)
#define	DevER88		(22088)
#define	DevER89		(22089)
#define	DevER90		(22090)
#define	DevER91		(22091)
#define	DevER92		(22092)
#define	DevER93		(22093)
#define	DevER94		(22094)
#define	DevER95		(22095)
#define	DevER96		(22096)
#define	DevER97		(22097)
#define	DevER98		(22098)
#define	DevER99		(22099)
#define	DevER100	(22100)
#define	DevER101	(22101)
#define	DevER102	(22102)
#define	DevER103	(22103)
#define	DevER104	(22104)
#define	DevER105	(22105)
#define	DevER106	(22106)
#define	DevER107	(22107)
#define	DevER108	(22108)
#define	DevER109	(22109)
#define	DevER110	(22110)
#define	DevER111	(22111)
#define	DevER112	(22112)
#define	DevER113	(22113)
#define	DevER114	(22114)
#define	DevER115	(22115)
#define	DevER116	(22116)
#define	DevER117	(22117)
#define	DevER118	(22118)
#define	DevER119	(22119)
#define	DevER120	(22120)
#define	DevER121	(22121)
#define	DevER122	(22122)
#define	DevER123	(22123)
#define	DevER124	(22124)
#define	DevER125	(22125)
#define	DevER126	(22126)
#define	DevER127	(22127)
#define	DevER128	(22128)
#define	DevER129	(22129)
#define	DevER130	(22130)
#define	DevER131	(22131)
#define	DevER132	(22132)
#define	DevER133	(22133)
#define	DevER134	(22134)
#define	DevER135	(22135)
#define	DevER136	(22136)
#define	DevER137	(22137)
#define	DevER138	(22138)
#define	DevER139	(22139)
#define	DevER140	(22140)
#define	DevER141	(22141)
#define	DevER142	(22142)
#define	DevER143	(22143)
#define	DevER144	(22144)
#define	DevER145	(22145)
#define	DevER146	(22146)
#define	DevER147	(22147)
#define	DevER148	(22148)
#define	DevER149	(22149)
#define	DevER150	(22150)
#define	DevER151	(22151)
#define	DevER152	(22152)
#define	DevER153	(22153)
#define	DevER154	(22154)
#define	DevER155	(22155)
#define	DevER156	(22156)
#define	DevER157	(22157)
#define	DevER158	(22158)
#define	DevER159	(22159)
#define	DevER160	(22160)
#define	DevER161	(22161)
#define	DevER162	(22162)
#define	DevER163	(22163)
#define	DevER164	(22164)
#define	DevER165	(22165)
#define	DevER166	(22166)
#define	DevER167	(22167)
#define	DevER168	(22168)
#define	DevER169	(22169)
#define	DevER170	(22170)
#define	DevER171	(22171)
#define	DevER172	(22172)
#define	DevER173	(22173)
#define	DevER174	(22174)
#define	DevER175	(22175)
#define	DevER176	(22176)
#define	DevER177	(22177)
#define	DevER178	(22178)
#define	DevER179	(22179)
#define	DevER180	(22180)
#define	DevER181	(22181)
#define	DevER182	(22182)
#define	DevER183	(22183)
#define	DevER184	(22184)
#define	DevER185	(22185)
#define	DevER186	(22186)
#define	DevER187	(22187)
#define	DevER188	(22188)
#define	DevER189	(22189)
#define	DevER190	(22190)
#define	DevER191	(22191)
#define	DevER192	(22192)
#define	DevER193	(22193)
#define	DevER194	(22194)
#define	DevER195	(22195)
#define	DevER196	(22196)
#define	DevER197	(22197)
#define	DevER198	(22198)
#define	DevER199	(22199)
#define	DevER200	(22200)
#define	DevER201	(22201)
#define	DevER202	(22202)
#define	DevER203	(22203)
#define	DevER204	(22204)
#define	DevER205	(22205)
#define	DevER206	(22206)
#define	DevER207	(22207)
#define	DevER208	(22208)
#define	DevER209	(22209)
#define	DevER210	(22210)
#define	DevER211	(22211)
#define	DevER212	(22212)
#define	DevER213	(22213)
#define	DevER214	(22214)
#define	DevER215	(22215)
#define	DevER216	(22216)
#define	DevER217	(22217)
#define	DevER218	(22218)
#define	DevER219	(22219)
#define	DevER220	(22220)
#define	DevER221	(22221)
#define	DevER222	(22222)
#define	DevER223	(22223)
#define	DevER224	(22224)
#define	DevER225	(22225)
#define	DevER226	(22226)
#define	DevER227	(22227)
#define	DevER228	(22228)
#define	DevER229	(22229)
#define	DevER230	(22230)
#define	DevER231	(22231)
#define	DevER232	(22232)
#define	DevER233	(22233)
#define	DevER234	(22234)
#define	DevER235	(22235)
#define	DevER236	(22236)
#define	DevER237	(22237)
#define	DevER238	(22238)
#define	DevER239	(22239)
#define	DevER240	(22240)
#define	DevER241	(22241)
#define	DevER242	(22242)
#define	DevER243	(22243)
#define	DevER244	(22244)
#define	DevER245	(22245)
#define	DevER246	(22246)
#define	DevER247	(22247)
#define	DevER248	(22248)
#define	DevER249	(22249)
#define	DevER250	(22250)
#define	DevER251	(22251)
#define	DevER252	(22252)
#define	DevER253	(22253)
#define	DevER254	(22254)
#define	DevER255	(22255)
#define	DevER256	(22256)
#define	DevRBM		(-32768)
#define	DevRAB		(-32736)
#define	DevRX		(-32735)
#define	DevRY		(-32734)
#define	DevRW		(-32732)
#define	DevARB		(-32704)
#define	DevSB		(-32669)
#define	DevSW		(-32668)
#define	DevEM(x)	(31*1000+(x))
#define	DevED(x)	(32*1000+(x))

