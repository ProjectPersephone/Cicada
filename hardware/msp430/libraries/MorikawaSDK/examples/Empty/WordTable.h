/*
**      ARTSAT Despatch Application
**
**      Original Copyright (C) 2014 - 2014 HORIGUCHI Junshi.
**                                          http://iridium.jp/
**                                          zap00365@nifty.com
**      Portions Copyright (C) <year> <author>
**                                          <website>
**                                          <e-mail>
**      All rights reserved.
**
**      Version     Arduino
**      Website     http://artsat.jp/
**      E-mail      info@artsat.jp
**
**      This source code is for Arduino IDE.
**      Arduino 1.0.5
**
**      WordTable.h
**
**      ------------------------------------------------------------------------
**
**      GNU GENERAL PUBLIC LICENSE (GPLv3)
**
**      This program is free software: you can redistribute it and/or modify
**      it under the terms of the GNU General Public License as published by
**      the Free Software Foundation, either version 3 of the License,
**      or (at your option) any later version.
**      This program is distributed in the hope that it will be useful,
**      but WITHOUT ANY WARRANTY; without even the implied warranty of
**      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
**      See the GNU General Public License for more details.
**      You should have received a copy of the GNU General Public License
**      along with this program. If not, see <http://www.gnu.org/licenses/>.
**
**      このプログラムはフリーソフトウェアです。あなたはこれをフリーソフトウェア財団によって発行された
**      GNU 一般公衆利用許諾書（バージョン 3 か、それ以降のバージョンのうちどれか）が定める条件の下で
**      再頒布または改変することができます。このプログラムは有用であることを願って頒布されますが、
**      *全くの無保証* です。商業可能性の保証や特定目的への適合性は、言外に示されたものも含め全く存在しません。
**      詳しくは GNU 一般公衆利用許諾書をご覧ください。
**      あなたはこのプログラムと共に GNU 一般公衆利用許諾書のコピーを一部受け取っているはずです。
**      もし受け取っていなければ <http://www.gnu.org/licenses/> をご覧ください。
*/

#ifndef __WORDTABLE_H
#define __WORDTABLE_H

#define UNPROGMEM // until this issue is understood
#define UNPGM_P const char *

static char const g_color_000[] UNPROGMEM = "whit";
static char const g_color_001[] UNPROGMEM = "whit";
static char const g_color_002[] UNPROGMEM = "whit";
static char const g_color_003[] UNPROGMEM = "whit";
static char const g_color_004[] UNPROGMEM = "whit";
static char const g_color_005[] UNPROGMEM = "whit";
static char const g_color_006[] UNPROGMEM = "whit";
static char const g_color_007[] UNPROGMEM = "whit";
static char const g_color_008[] UNPROGMEM = "hlot";
static char const g_color_009[] UNPROGMEM = "mulb";
static char const g_color_010[] UNPROGMEM = "prpl";
static char const g_color_011[] UNPROGMEM = "volt";
static char const g_color_012[] UNPROGMEM = "blvl";
static char const g_color_013[] UNPROGMEM = "indg";
static char const g_color_014[] UNPROGMEM = "pers";
static char const g_color_015[] UNPROGMEM = "blue";
static char const g_color_016[] UNPROGMEM = "sprb";
static char const g_color_017[] UNPROGMEM = "sphr";
static char const g_color_018[] UNPROGMEM = "cblt";
static char const g_color_019[] UNPROGMEM = "azur";
static char const g_color_020[] UNPROGMEM = "crnf";
static char const g_color_021[] UNPROGMEM = "cerl";
static char const g_color_022[] UNPROGMEM = "arct";
static char const g_color_023[] UNPROGMEM = "cyan";
static char const g_color_024[] UNPROGMEM = "opal";
static char const g_color_025[] UNPROGMEM = "trqs";
static char const g_color_026[] UNPROGMEM = "aqmr";
static char const g_color_027[] UNPROGMEM = "sprg";
static char const g_color_028[] UNPROGMEM = "seag";
static char const g_color_029[] UNPROGMEM = "mlct";
static char const g_color_030[] UNPROGMEM = "emrd";
static char const g_color_031[] UNPROGMEM = "gren";
static char const g_color_032[] UNPROGMEM = "sapg";
static char const g_color_033[] UNPROGMEM = "hrlq";
static char const g_color_034[] UNPROGMEM = "pstc";
static char const g_color_035[] UNPROGMEM = "chtr";
static char const g_color_036[] UNPROGMEM = "sprg";
static char const g_color_037[] UNPROGMEM = "lime";
static char const g_color_038[] UNPROGMEM = "appl";
static char const g_color_039[] UNPROGMEM = "yelw";
static char const g_color_040[] UNPROGMEM = "gold";
static char const g_color_041[] UNPROGMEM = "ambr";
static char const g_color_042[] UNPROGMEM = "gmbg";
static char const g_color_043[] UNPROGMEM = "orag";
static char const g_color_044[] UNPROGMEM = "tngl";
static char const g_color_045[] UNPROGMEM = "vrmn";
static char const g_color_046[] UNPROGMEM = "sclt";
static char const g_color_047[] UNPROGMEM = "red_";
static char const g_color_048[] UNPROGMEM = "amrt";
static char const g_color_049[] UNPROGMEM = "crms";
static char const g_color_050[] UNPROGMEM = "rsbr";
static char const g_color_051[] UNPROGMEM = "rose";
static char const g_color_052[] UNPROGMEM = "cers";
static char const g_color_053[] UNPROGMEM = "fcha";
static char const g_color_054[] UNPROGMEM = "orcd";
static char const g_color_055[] UNPROGMEM = "mgnt";
static char const g_color_056[] UNPROGMEM = "blck";
static char const g_color_057[] UNPROGMEM = "blck";
static char const g_color_058[] UNPROGMEM = "blck";
static char const g_color_059[] UNPROGMEM = "blck";
static char const g_color_060[] UNPROGMEM = "blck";
static char const g_color_061[] UNPROGMEM = "blck";
static char const g_color_062[] UNPROGMEM = "blck";
static char const g_color_063[] UNPROGMEM = "blck";
static UNPGM_P const g_color[] UNPROGMEM = {
    g_color_000,
    g_color_001,
    g_color_002,
    g_color_003,
    g_color_004,
    g_color_005,
    g_color_006,
    g_color_007,
    g_color_008,
    g_color_009,
    g_color_010,
    g_color_011,
    g_color_012,
    g_color_013,
    g_color_014,
    g_color_015,
    g_color_016,
    g_color_017,
    g_color_018,
    g_color_019,
    g_color_020,
    g_color_021,
    g_color_022,
    g_color_023,
    g_color_024,
    g_color_025,
    g_color_026,
    g_color_027,
    g_color_028,
    g_color_029,
    g_color_030,
    g_color_031,
    g_color_032,
    g_color_033,
    g_color_034,
    g_color_035,
    g_color_036,
    g_color_037,
    g_color_038,
    g_color_039,
    g_color_040,
    g_color_041,
    g_color_042,
    g_color_043,
    g_color_044,
    g_color_045,
    g_color_046,
    g_color_047,
    g_color_048,
    g_color_049,
    g_color_050,
    g_color_051,
    g_color_052,
    g_color_053,
    g_color_054,
    g_color_055,
    g_color_056,
    g_color_057,
    g_color_058,
    g_color_059,
    g_color_060,
    g_color_061,
    g_color_062,
    g_color_063
};
static char const g_rhythm_000[] UNPROGMEM = "abim";
static char const g_rhythm_001[] UNPROGMEM = "adji";
static char const g_rhythm_002[] UNPROGMEM = "affa";
static char const g_rhythm_003[] UNPROGMEM = "alla";
static char const g_rhythm_004[] UNPROGMEM = "amai";
static char const g_rhythm_005[] UNPROGMEM = "amen";
static char const g_rhythm_006[] UNPROGMEM = "amma";
static char const g_rhythm_007[] UNPROGMEM = "assa";
static char const g_rhythm_008[] UNPROGMEM = "aula";
static char const g_rhythm_009[] UNPROGMEM = "auli";
static char const g_rhythm_010[] UNPROGMEM = "bala";
static char const g_rhythm_011[] UNPROGMEM = "balo";
static char const g_rhythm_012[] UNPROGMEM = "band";
static char const g_rhythm_013[] UNPROGMEM = "bang";
static char const g_rhythm_014[] UNPROGMEM = "bann";
static char const g_rhythm_015[] UNPROGMEM = "beri";
static char const g_rhythm_016[] UNPROGMEM = "bimb";
static char const g_rhythm_017[] UNPROGMEM = "bimm";
static char const g_rhythm_018[] UNPROGMEM = "binb";
static char const g_rhythm_019[] UNPROGMEM = "binn";
static char const g_rhythm_020[] UNPROGMEM = "blas";
static char const g_rhythm_021[] UNPROGMEM = "blau";
static char const g_rhythm_022[] UNPROGMEM = "blon";
static char const g_rhythm_023[] UNPROGMEM = "bluk";
static char const g_rhythm_024[] UNPROGMEM = "blun";
static char const g_rhythm_025[] UNPROGMEM = "brus";
static char const g_rhythm_026[] UNPROGMEM = "bulo";
static char const g_rhythm_027[] UNPROGMEM = "bumb";
static char const g_rhythm_028[] UNPROGMEM = "cado";
static char const g_rhythm_029[] UNPROGMEM = "dibl";
static char const g_rhythm_030[] UNPROGMEM = "dibu";
static char const g_rhythm_031[] UNPROGMEM = "didi";
static char const g_rhythm_032[] UNPROGMEM = "diga";
static char const g_rhythm_033[] UNPROGMEM = "dori";
static char const g_rhythm_034[] UNPROGMEM = "drid";
static char const g_rhythm_035[] UNPROGMEM = "elif";
static char const g_rhythm_036[] UNPROGMEM = "falo";
static char const g_rhythm_037[] UNPROGMEM = "fant";
static char const g_rhythm_038[] UNPROGMEM = "gadj";
static char const g_rhythm_039[] UNPROGMEM = "gado";
static char const g_rhythm_040[] UNPROGMEM = "gaga";
static char const g_rhythm_041[] UNPROGMEM = "gala";
static char const g_rhythm_042[] UNPROGMEM = "glan";
static char const g_rhythm_043[] UNPROGMEM = "glas";
static char const g_rhythm_044[] UNPROGMEM = "glig";
static char const g_rhythm_045[] UNPROGMEM = "gooo";
static char const g_rhythm_046[] UNPROGMEM = "gram";
static char const g_rhythm_047[] UNPROGMEM = "hoge";
static char const g_rhythm_048[] UNPROGMEM = "hooo";
static char const g_rhythm_049[] UNPROGMEM = "hops";
static char const g_rhythm_050[] UNPROGMEM = "ibar";
static char const g_rhythm_051[] UNPROGMEM = "igla";
static char const g_rhythm_052[] UNPROGMEM = "imai";
static char const g_rhythm_053[] UNPROGMEM = "imba";
static char const g_rhythm_054[] UNPROGMEM = "imzi";
static char const g_rhythm_055[] UNPROGMEM = "inai";
static char const g_rhythm_056[] UNPROGMEM = "iola";
static char const g_rhythm_057[] UNPROGMEM = "ital";
static char const g_rhythm_058[] UNPROGMEM = "izim";
static char const g_rhythm_059[] UNPROGMEM = "jama";
static char const g_rhythm_060[] UNPROGMEM = "jame";
static char const g_rhythm_061[] UNPROGMEM = "kata";
static char const g_rhythm_062[] UNPROGMEM = "lala";
static char const g_rhythm_063[] UNPROGMEM = "laul";
static char const g_rhythm_064[] UNPROGMEM = "laxa";
static char const g_rhythm_065[] UNPROGMEM = "leng";
static char const g_rhythm_066[] UNPROGMEM = "lima";
static char const g_rhythm_067[] UNPROGMEM = "ling";
static char const g_rhythm_068[] UNPROGMEM = "lita";
static char const g_rhythm_069[] UNPROGMEM = "llal";
static char const g_rhythm_070[] UNPROGMEM = "lomi";
static char const g_rhythm_071[] UNPROGMEM = "long";
static char const g_rhythm_072[] UNPROGMEM = "lonn";
static char const g_rhythm_073[] UNPROGMEM = "looo";
static char const g_rhythm_074[] UNPROGMEM = "lopi";
static char const g_rhythm_075[] UNPROGMEM = "luji";
static char const g_rhythm_076[] UNPROGMEM = "luku";
static char const g_rhythm_077[] UNPROGMEM = "lung";
static char const g_rhythm_078[] UNPROGMEM = "malo";
static char const g_rhythm_079[] UNPROGMEM = "mbal";
static char const g_rhythm_080[] UNPROGMEM = "mbra";
static char const g_rhythm_081[] UNPROGMEM = "mini";
static char const g_rhythm_082[] UNPROGMEM = "mzim";
static char const g_rhythm_083[] UNPROGMEM = "nban";
static char const g_rhythm_084[] UNPROGMEM = "ndri";
static char const g_rhythm_085[] UNPROGMEM = "negr";
static char const g_rhythm_086[] UNPROGMEM = "noze";
static char const g_rhythm_087[] UNPROGMEM = "ogro";
static char const g_rhythm_088[] UNPROGMEM = "olim";
static char const g_rhythm_089[] UNPROGMEM = "omen";
static char const g_rhythm_090[] UNPROGMEM = "omin";
static char const g_rhythm_091[] UNPROGMEM = "onni";
static char const g_rhythm_092[] UNPROGMEM = "oooo";
static char const g_rhythm_093[] UNPROGMEM = "orsu";
static char const g_rhythm_094[] UNPROGMEM = "osso";
static char const g_rhythm_095[] UNPROGMEM = "ozer";
static char const g_rhythm_096[] UNPROGMEM = "palo";
static char const g_rhythm_097[] UNPROGMEM = "palu";
static char const g_rhythm_098[] UNPROGMEM = "pimp";
static char const g_rhythm_099[] UNPROGMEM = "pinx";
static char const g_rhythm_100[] UNPROGMEM = "piyo";
static char const g_rhythm_101[] UNPROGMEM = "purz";
static char const g_rhythm_102[] UNPROGMEM = "rabi";
static char const g_rhythm_103[] UNPROGMEM = "rhin";
static char const g_rhythm_104[] UNPROGMEM = "rida";
static char const g_rhythm_105[] UNPROGMEM = "ridi";
static char const g_rhythm_106[] UNPROGMEM = "ross";
static char const g_rhythm_107[] UNPROGMEM = "sala";
static char const g_rhythm_108[] UNPROGMEM = "sass";
static char const g_rhythm_109[] UNPROGMEM = "sola";
static char const g_rhythm_110[] UNPROGMEM = "ssas";
static char const g_rhythm_111[] UNPROGMEM = "tata";
static char const g_rhythm_112[] UNPROGMEM = "teru";
static char const g_rhythm_113[] UNPROGMEM = "torr";
static char const g_rhythm_114[] UNPROGMEM = "trom";
static char const g_rhythm_115[] UNPROGMEM = "tuff";
static char const g_rhythm_116[] UNPROGMEM = "uffm";
static char const g_rhythm_117[] UNPROGMEM = "urul";
static char const g_rhythm_118[] UNPROGMEM = "velo";
static char const g_rhythm_119[] UNPROGMEM = "viol";
static char const g_rhythm_120[] UNPROGMEM = "wowo";
static char const g_rhythm_121[] UNPROGMEM = "xato";
static char const g_rhythm_122[] UNPROGMEM = "zall";
static char const g_rhythm_123[] UNPROGMEM = "zamm";
static char const g_rhythm_124[] UNPROGMEM = "zanz";
static char const g_rhythm_125[] UNPROGMEM = "zimb";
static char const g_rhythm_126[] UNPROGMEM = "zimz";
static char const g_rhythm_127[] UNPROGMEM = "zing";
static UNPGM_P const g_rhythm[] UNPROGMEM = {
    g_rhythm_000,
    g_rhythm_001,
    g_rhythm_002,
    g_rhythm_003,
    g_rhythm_004,
    g_rhythm_005,
    g_rhythm_006,
    g_rhythm_007,
    g_rhythm_008,
    g_rhythm_009,
    g_rhythm_010,
    g_rhythm_011,
    g_rhythm_012,
    g_rhythm_013,
    g_rhythm_014,
    g_rhythm_015,
    g_rhythm_016,
    g_rhythm_017,
    g_rhythm_018,
    g_rhythm_019,
    g_rhythm_020,
    g_rhythm_021,
    g_rhythm_022,
    g_rhythm_023,
    g_rhythm_024,
    g_rhythm_025,
    g_rhythm_026,
    g_rhythm_027,
    g_rhythm_028,
    g_rhythm_029,
    g_rhythm_030,
    g_rhythm_031,
    g_rhythm_032,
    g_rhythm_033,
    g_rhythm_034,
    g_rhythm_035,
    g_rhythm_036,
    g_rhythm_037,
    g_rhythm_038,
    g_rhythm_039,
    g_rhythm_040,
    g_rhythm_041,
    g_rhythm_042,
    g_rhythm_043,
    g_rhythm_044,
    g_rhythm_045,
    g_rhythm_046,
    g_rhythm_047,
    g_rhythm_048,
    g_rhythm_049,
    g_rhythm_050,
    g_rhythm_051,
    g_rhythm_052,
    g_rhythm_053,
    g_rhythm_054,
    g_rhythm_055,
    g_rhythm_056,
    g_rhythm_057,
    g_rhythm_058,
    g_rhythm_059,
    g_rhythm_060,
    g_rhythm_061,
    g_rhythm_062,
    g_rhythm_063,
    g_rhythm_064,
    g_rhythm_065,
    g_rhythm_066,
    g_rhythm_067,
    g_rhythm_068,
    g_rhythm_069,
    g_rhythm_070,
    g_rhythm_071,
    g_rhythm_072,
    g_rhythm_073,
    g_rhythm_074,
    g_rhythm_075,
    g_rhythm_076,
    g_rhythm_077,
    g_rhythm_078,
    g_rhythm_079,
    g_rhythm_080,
    g_rhythm_081,
    g_rhythm_082,
    g_rhythm_083,
    g_rhythm_084,
    g_rhythm_085,
    g_rhythm_086,
    g_rhythm_087,
    g_rhythm_088,
    g_rhythm_089,
    g_rhythm_090,
    g_rhythm_091,
    g_rhythm_092,
    g_rhythm_093,
    g_rhythm_094,
    g_rhythm_095,
    g_rhythm_096,
    g_rhythm_097,
    g_rhythm_098,
    g_rhythm_099,
    g_rhythm_100,
    g_rhythm_101,
    g_rhythm_102,
    g_rhythm_103,
    g_rhythm_104,
    g_rhythm_105,
    g_rhythm_106,
    g_rhythm_107,
    g_rhythm_108,
    g_rhythm_109,
    g_rhythm_110,
    g_rhythm_111,
    g_rhythm_112,
    g_rhythm_113,
    g_rhythm_114,
    g_rhythm_115,
    g_rhythm_116,
    g_rhythm_117,
    g_rhythm_118,
    g_rhythm_119,
    g_rhythm_120,
    g_rhythm_121,
    g_rhythm_122,
    g_rhythm_123,
    g_rhythm_124,
    g_rhythm_125,
    g_rhythm_126,
    g_rhythm_127
};

#endif
