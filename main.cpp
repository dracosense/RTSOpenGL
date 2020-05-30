#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <GL\glut.h>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <set>
#include <map>
//#include <bits/stdc++.h>

#define llong long long
#define inmenu(x, y) (x >= -0.096f && x < 0.096f && y >= -0.073f && y < 0.07f && (x >= 0.056f || y < -0.06f || x < -0.083f || y >= 0.064f))
#define outsidemenu(x, y) (x >= 0.096f || x < -0.096f || y >= 0.07f || y < -0.073f)
#define getmenuminimapx(x) (((x - 0.057f) * gmapx) / (min(0.04f / gmapx, 0.04f / gmapy) * gminimapx))
#define getmenuminimapy(y) (((y - 0.033f) * gmapy) / (min(0.04f / gmapx, 0.04f / gmapy) * gminimapy))

using namespace std;

const GLfloat msqx = 1.0f;
const GLfloat msqy = 1.0f;
const GLfloat msqz = 1.0f;
const GLfloat mdeep = 36.0f;
const GLint mviewx = 28;
const GLint mviewy = 25;
const GLint texturenum = 0;
const GLint inttextmax = 0;
const GLint gmapparamnum = 3; // place, player, player place
const GLint shsize = 1;
const GLint resourcenum = 1;
const GLint rpsize = 2; // resources type, resources num

GLuint texture[texturenum];
GLfloat startstring[3] = {0.0f, 0.0f, 0.0f};
GLfloat mp[2] = {0.0f, 0.0f};
GLfloat wp[2] = {800.0f, 600.0f};
pair<bool, bool> mb[3] = {{false, false}, {false, false}, {false, false}};
pair<GLfloat, GLfloat> mbpos[2][3] = {{{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}}, {{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}}};

bool addtime = false;
GLint dtime = 0;
GLint mgametime = 0;

GLint playernum = 2;
GLint actplayer = 0;
GLint mdrawx = 50;
GLint mdrawy = 50;
GLint gmapx = 101;
GLint gmapy = 101;
GLint gminimapx = 101;
GLint gminimapy = 101;
GLfloat gminimapres = 10; // res level on minimap square for res type
GLint mapmovetimer = 10;
GLint mapmovetime = 100;
GLint gmapposx = 0;
GLint gmapposy = 0;

vector<GLint> worldid(playernum, 0);

class pos
{
public:
    GLfloat x, y;
    pos()
    {
        x = 0.0f;
        y = 0.0f;
    }
    ~pos()
    {

    }
};

class res
{
public:
    GLint n;
    res(GLint n2 = 0)
    {
        n = n2;
    }
    ~res()
     {

     }
};

class resp
{
public:
    GLint w; // weight
    resp(GLint w2 = 0)
    {
        w = w2;
    }
    ~resp()
    {

    }
};

vector<vector<res> > presources(playernum, vector<res> (resourcenum));

vector<resp> resparam(resourcenum);

vector<vector<GLint> > pactresources(playernum);

GLfloat distf(pair<GLfloat, GLfloat> a, pair<GLfloat, GLfloat> b)
{
    return sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second));
}

void setminimapres(GLint i, GLint j);

void sendactopoint(GLint x, GLint y);

void square2d(GLfloat X, GLfloat Y, GLfloat X2, GLfloat Y2)
{
	glBegin(GL_QUADS);
	glVertex2d(X, Y);
	glVertex2d(X, Y2);
	glVertex2d(X2, Y2);
	glVertex2d(X2, Y);
	glEnd();

}

void square(GLfloat X, GLfloat Y, GLfloat Z, GLfloat X2, GLfloat Y2, GLfloat Z2, GLfloat R, GLfloat G, GLfloat B, GLfloat A)
{
	glBegin(GL_QUADS);
	glColor4f(R, G, B, A);
	glVertex3d(X, Y, Z);
	if (X == X2)
	{
		glVertex3d(X, Y2, Z);
		glVertex3d(X2, Y2, Z2);
		glVertex3d(X, Y, Z2);
	}
	if (Y == Y2)
	{
		glVertex3d(X2, Y, Z);
		glVertex3d(X2, Y2, Z2);
		glVertex3d(X, Y, Z2);
	}
	if (Z == Z2)
	{
		glVertex3d(X2, Y, Z);
		glVertex3d(X2, Y2, Z2);
		glVertex3d(X, Y2, Z);
	}
	glEnd();
}

void square(GLfloat X, GLfloat Y, GLfloat Z, GLfloat X2, GLfloat Y2, GLfloat Z2, GLfloat R, GLfloat G, GLfloat B)
{
	square(X, Y, Z, X2, Y2, Z2, R, G, B, 1.0f);
}

void square(GLfloat X, GLfloat Y, GLfloat Z, GLfloat R, GLfloat G, GLfloat B)
{
	square(0.0f, 0.0f, 0.0f, X, Y, Z, R, G ,B);
}

void square(GLfloat X, GLfloat Y, GLfloat Z, GLfloat R, GLfloat G, GLfloat B, GLfloat A)
{
	square(0.0f, 0.0f, 0.0f, X, Y, Z, R, G, B, A);
}

void tsquare(GLint tname, GLfloat X, GLfloat Y, GLfloat Z, GLfloat X2, GLfloat Y2, GLfloat Z2,
			GLfloat X3, GLfloat Y3, GLfloat Z3, GLfloat X4, GLfloat Y4, GLfloat Z4)
{
	glBindTexture(GL_TEXTURE_2D, texture[tname]);
	glBegin(GL_QUADS);
	glTexCoord2f (0.0f, 0.0f); glVertex3d (X, Y, Z);
	glTexCoord2f (0.0f, 1.0f); glVertex3d (X2, Y2, Z2);
	glTexCoord2f (1.0f, 1.0f); glVertex3d (X3, Y3, Z3);
	glTexCoord2f (1.0f, 0.0f); glVertex3d (X4, Y4, Z4);
	glEnd();
}

void tsquare(GLfloat X, GLfloat Y, GLfloat Z,  GLfloat X2, GLfloat Y2, GLfloat Z2)
{
	glBegin (GL_QUADS);
	glTexCoord2f (0.0f, 0.0f); glVertex3d (X, Y, Z);
	if (X == X2)
	{
		glTexCoord2f (0.0f, 1.0f); glVertex3d (X, Y2, Z);
		glTexCoord2f (1.0f, 1.0f); glVertex3d (X, Y2, Z2);
		glTexCoord2f (1.0f, 0.0f); glVertex3d (X, Y, Z2);
	}
	if (Y == Y2)
	{
		glTexCoord2f (0.0f, 1.0f); glVertex3d (X2, Y, Z);
		glTexCoord2f (1.0f, 1.0f); glVertex3d (X2, Y, Z2);
		glTexCoord2f (1.0f, 0.0f); glVertex3d (X, Y, Z2);
	}
	if (Z == Z2)
	{
		glTexCoord2f (0.0f, 1.0f); glVertex3d (X, Y2, Z);
		glTexCoord2f (1.0f, 1.0f); glVertex3d (X2, Y2, Z);
		glTexCoord2f (1.0f, 0.0f); glVertex3d (X2, Y, Z);
	}
	glEnd ();
}

void tsquare(GLfloat X, GLfloat Y, GLfloat X2, GLfloat Y2)
{
	if (X2 < X)
	{
		swap (X, X2);
	}
	if (Y2 < Y)
	{
		swap (Y2, Y);
	}
	glBegin (GL_QUADS);
	glTexCoord2f (0.0f, 0.0f); glVertex2d (X, Y);
	glTexCoord2f (0.0f, 1.0f); glVertex2d (X, Y2);
	glTexCoord2f (1.0f, 1.0f); glVertex2d (X2, Y2);
	glTexCoord2f (1.0f, 0.0f); glVertex2d (X2, Y);
	glEnd ();
}

void line(GLfloat X, GLfloat Y, GLfloat Z, GLfloat X2, GLfloat Y2, GLfloat Z2, GLfloat width)
{
	glLineWidth (width);
	glBegin(GL_LINES);
	glVertex3d(X, Y, Z);
	glVertex3d(X2, Y2, Z2);
	glEnd();
}

void line(GLfloat X, GLfloat Y, GLfloat X2, GLfloat Y2, GLfloat width)
{
	line(X, Y, 0.0f, X2, Y2, 0.0f, width);
}

void squareline(GLfloat X, GLfloat Y, GLfloat X2, GLfloat Y2, GLfloat W)
{
	line (X, Y, X, Y2, W);
	line (X, Y, X2, Y, W);
	line (X2, Y, X2, Y2, W);
	line (X, Y2, X2, Y2, W);
}

void tsquare(GLint tname, GLfloat X, GLfloat Y, GLfloat Z, GLfloat X2, GLfloat Y2, GLfloat Z2)
{
	glBindTexture (GL_TEXTURE_2D, texture[tname]);
	tsquare (X, Y, Z, X2, Y2, Z2);
}

void tsquare(GLint tname1, GLint tname2, GLfloat X, GLfloat Y, GLfloat Z, GLfloat X2, GLfloat Y2, GLfloat Z2)
{
	glEnable (GL_BLEND);
	glDisable (GL_DEPTH_TEST);
	glBlendFunc (GL_DST_COLOR, GL_ZERO);
	glBindTexture (GL_TEXTURE_2D, texture[tname1]);
	tsquare (X, Y, Z, X2, Y2, Z2);
	glBlendFunc (GL_ONE, GL_ONE);
	glBindTexture (GL_TEXTURE_2D, texture[tname2]);
	tsquare (X, Y, Z, X2, Y2, Z2);
	glEnable (GL_DEPTH_TEST);
	glDisable (GL_BLEND);
}

void tsquare(GLint tname, GLfloat X, GLfloat Y, GLfloat X2, GLfloat Y2)
{
	glBindTexture (GL_TEXTURE_2D, texture[tname]);
	tsquare (X, Y, X2, Y2);
}

void tsquare(GLint tname1, GLint tname2, GLfloat X, GLfloat Y, GLfloat X2, GLfloat Y2)
{
	tsquare (tname1, tname2, X, Y, 0.0f, X2, Y2, 0.0f);
}

class color
{
public:
    GLfloat r, g, b, a;
    color()
    {
        r = 0.0f;
        g = 0.0f;
        b = 0.0f;
        a = 1.0f;
    }
    void setc()
    {
        glColor4f(r, g, b, a);
    }
    ~color()
    {

    }
};

class melem
{
public:
    vector<GLint> param;
    vector<GLint> rp; // resource param
    melem()
    {
        param.resize(gmapparamnum, 0); // param[0] - 0 - place, 1 - thing, 2 - resources, param[1] - player, param[2] - player place
        rp.resize(rpsize, -1);
        param[1] = -1;
    }
    ~melem()
    {

    }
};

class mcolor
{
public:
    GLfloat R, G, B, A;
    mcolor()
    {
        R = 0.0f;
        G = 0.0f;
        B = 0.0f;
        A = 1.0f;
    }
    ~mcolor()
    {

    }
};

vector<vector<melem> > gmap; // resourses, things, creatures / fly creatures / ...

vector<vector<melem> > gminimap;

vector<mcolor> pcolor; // color for players

vector<GLint> buildtype;

vector<GLint> bdrawsx(playernum, 0);

vector<GLint> bdrawsy(playernum, 0);

vector<GLint> buildnum (playernum, 0);

void setminimapres(GLint i, GLint j)
{
    if ((GLfloat)gminimap[i / (gmapx / gminimapx)][j / (gmapy / gminimapy)].rp[1] / (((GLfloat)gmapx / (GLfloat)gminimapx) *((GLfloat)gmapy / (GLfloat)gminimapy)) >= gminimapres)
    {
        gminimap[i / (gmapx / gminimapx)][j / (gmapy / gminimapy)].param[0] = 2;
    }
    else
    {
        gminimap[i / (gmapx / gminimapx)][j / (gmapy / gminimapy)].param[0] = 0;
    }
}

bool buildobj(GLint posx, GLint posy, GLint sx, GLint sy, bool c = false, GLint objp = 0, GLint objn = 0)
{
    bool b  = true;
    if (posx < 0 || posy < 0 || posx + sx - 1 >= gmap.size() || posy + sy - 1 >= gmap[0].size())
    {
        b = false;
    }
    else
    {
        for (long i = posx; i < posx + sx; i++)
        {
            for (long j = posy; j < posy + sy; j++)
            {
                if (gmap[i][j].param[0] != 0 || gmap[i][j].param[1] != -1)
                {
                    b = false;
                }
                if(c)
                {
                    gmap[i][j].param[1] = objp;
                    gmap[i][j].param[2] = objn;
                }
            }
        }
    }
    return b;
}


class canon
{
public:
    GLint type, time, timer, range, atype, p, n; // type, time, timer, range of attack, attack type, player, num of obj
    GLint r; // range of destroy
    GLfloat s, attack; // speed of attack, attack
    canon(GLint type2 = 0, GLint time2 = 0, GLint range2 = 0, GLfloat attack2 = 0.0f, GLint atype2 = 0, GLint p2 = 0, GLint n2 = 0, GLfloat s2 = 2.0f, GLint r2 = 0)
    {
        type = type2;
        time = time2;
        attack = attack2;
        atype = atype2;
        range = range2;
        n = n2;
        p = p2;
        s = s2;
        r = r2;
        timer = 0;
    }

    bool getdist(GLfloat posx, GLfloat posy, GLint posx2, GLint posy2)
    {
        if (max(max(posx - posx2, posx2 - posx), max(posy - posy2, posy2 - posy)) <= range)
        {
            return true;
        }
        return false;
    }

    bool attackobj(GLint player, GLint obj, bool aobj); // aobj - attack / without attack

    ~canon()
    {

    }
};

//void attackobj(canon &c, GLfloat posx, GLfloat posy, GLfloat sx, GLfloat sy, GLint player, GLint obj);

class shieldelem
{
public:
    GLfloat a;
    shieldelem()
    {

    }
    ~shieldelem()
    {

    }
};

class bobj
{
public:
    GLint t, d; // d, s; // type, speed of building (k)
    bool in; // true - inside, false - outside
    vector<res> r; // resources
    bobj()
    {
        t = 0;
        in = false;
        r.resize(resourcenum);
    }
    ~bobj()
    {

    }
};

class cargo
{
public:
    GLint mw; // max weight sum for creatures;
    GLint w; // creatures weight
    vector<pair<res, GLint> > r; // resources: res, max resources num
    vector<pair<GLint, pair<GLint, GLint> > > t; // creature transport: player, n, weight
    cargo(GLint mw2 = 0)
    {
        res a;
        r.resize(resourcenum, {a, 0});
        mw = mw2;
        w = 0;
    }
    bool add(GLint player, GLint n, GLint weight) // add creature: player, n
    {
        if (w + weight <= mw)
        {
            t.push_back({player, {n, weight}});
            w += weight;
            return true;
        }
        else
        {
            return false;
        }
    }
    GLint add(GLint n, GLint p, GLint &getrtimer, GLint getrspeed) // add resources: num, type, get resources timer, get resources speed
    {
        GLint a = 0, b = 0;
        if (p < r.size())
        {
            a = min(r[p].second - r[p].first.n, n);
            b = (mgametime - getrtimer) / getrspeed;
            getrtimer = mgametime - ((mgametime - getrtimer) % getrspeed);
            r[p].first.n += min(a, b);
            if (n <= b)
            {
                getrtimer = mgametime;
            }
            return (n - min(a, b));
        }
        return -1;
    }
    bool resfull(GLint num = -1)
    {
        if (num == -1)
        {
            for (long i = 0; i < r.size(); i++)
            {
                if (r[i].first.n < r[i].second)
                {
                    return false;
                }
            }
            return true;
        }
        if (r[num].first.n == r[num].second)
        {
            return true;
        }
        return false;
    }
    bool resempty(GLint num = -1)
    {
        if (num == -1)
        {
            for (GLint i = 0; i < r.size(); i++)
            {
                if (r[i].first.n != 0)
                {
                    return false;
                }
            }
            return true;
        }
        if (r[num].first.n != 0)
        {
            return true;
        }
        return false;
    }
    void step();
    ~cargo()
    {

    }
};

class obj
{
public:
    GLfloat speed, bspeed; // speed, build speed
    GLint sx, sy, sm, weight, vrange, brange, rrange; // size x, size y, start modification, start live, weight, view range, build range, resources range
    GLint pgrspeed, getrspeed, sendrspeed, sendrrange; // player get resources speed (milliseconds), get resources speed (milliseconds), send resource speed (milliseconds), send resource range
    GLint inrange; // go inside range
    string name;
    cargo tplace; // transport place
    bool getresbase; // get resources for player
    vector<canon> w;
    vector<shieldelem> sh;
    vector<bobj> bt;
    obj(GLint sx2 = 1, GLint sy2 = 1, GLint slive = 100, GLint sm2 = 0, GLfloat speed2 = 200, GLfloat bspeed2 = 1.0f, GLint weight2 = 0, GLint vrange2 = 0, GLint brange2 = 0, GLint rrange2 = 0, GLint pgrspeed2 = 0, GLint getrspeed2 = 0, GLint sendrspeed2 = 0, GLint sendrrange2 = 0, GLint inrange2 = 0, string name2 = "")
    {
        sx = sx2;
        sy = sy2;
        sm = sm2;
        speed = speed2;
        sh.resize(shsize);
        sh[0].a = slive;
        bspeed = bspeed2;
        weight = weight2;
        vrange = vrange2;
        brange = brange2;
        rrange = rrange2;
        pgrspeed = pgrspeed2;
        getrspeed = getrspeed2;
        sendrspeed = sendrspeed2;
        sendrrange = sendrrange2;
        name = name2;
        inrange = inrange2;
    }
    ~obj()
    {

    }
};

class creature
{
public:
    bool step, active;
    bool insidec, outsidec; // inside other creature, outside other creature(for build and destroy)
    GLfloat rposx, rposy, buildp, bspeed; // real pos x, real pos y, build progress, build speed
    GLint sx, sy, posx, posy, m, t, act, stept, px, py, nx, ny, speed, attackobj, attackplayer, pposx, pposy; // size x, size y, pos x, pos y, modification, type, action, step timer, previous pos x(for go), previous pos y(for go), next pos x, next pos y, speed, attack pos x, attack pos y, previous posx, previous pos y
    GLint thisp, thispn, bcreature, slive, btime, btimer; // this player, this player num, start live, build time, build timer
    GLint vrange, brange, rrange, weight, bmapx, bmapy, buildt, pact; // view range, build range, resources range, weight, build map x pos, build map y pos, type to build, previous act
    GLint pgposx, pgposy, getrestime, restype; // previous go pos x, previous go pos y, get resources time, type to find resources
    GLint pgrspeed, getrspeed, pgrtimer, getrtimer; // player get resources speed (milliseconds), get resources speed (milliseconds), timer for player get resources, get resource timer
    GLint rbasex, rbasey, sendrtimer, sendrspeed, sendrrange, rbasen; // resource base position x, resource base position y, send resource timer, send resource speed (milliseconds), send resources range, resource base n
    GLint inrange; // go inside range
    string name;
    cargo tplace; // transport place
    pair<GLfloat, GLfloat> steppos;
    pair<GLint, GLint> gopos;
    pair<GLint, GLint> rpos; // resource position
    set<pair<llong, llong> > waypos;
    vector<canon> weapon;
    vector<shieldelem> shield;
    vector<bobj> btype; // build type
    creature(GLint sx2 = 1, GLint sy2 = 1, GLint posx2 = 0, GLint posy2 = 0, GLint m2 = 0, GLint t2 = 0, GLint thisp2 = 0, GLint thispn2 = 0, GLfloat speed2 = 200,
             GLfloat buildp2 = 1.0f, GLint live = 100, GLint slive2 = 100, GLfloat bspeed2 = 1.0f, GLint weight2 = 0, GLint vrange2 = -1, GLint brange2 = -1,
             GLint rrange2 = -1, GLint pgrspeed2 = 0, GLint getrspeed2 = 0, GLint sendrspeed2 = 0, GLint sendrrange2 = 0, GLint inrange2 = 0, string name2 = "")//, vector<canon> &weapon2, vector<shieldelem> &shield2)
    {
        posx = posx2;
        posy = posy2;
        nx = posx;
        ny = posy;
        px = posx;
        py = posy;
        rposx = posx;
        rposy = posy;
        pposx = -1;
        pposy = -1;
        pgposx = -1;
        pgposy = -1;
        steppos = {0.0f, 0.0f};
        stept = true;
        step = true;
        active = false;
        sx = sx2;
        sy = sy2;
        m = m2;
        t = t2;
        act = -1;
        thisp = thisp2;
        thispn = thispn2;
        speed = speed2;
        buildp = buildp2;
        attackobj = -1;
        attackplayer = -1;
        bcreature = 0;
        slive = slive2;
        bspeed = bspeed2;
        btime = 0;
        btimer = 1000;
        shield.resize(shsize);
        shield[0].a = live;
        weight = weight2;
        vrange = vrange2;
        brange = brange2;
        rrange = rrange2;
        bmapx = 0;
        bmapy = 0;
        buildt = -1;
        pact = -1;
        insidec = false;
        outsidec = true;
        rpos = {-1, -1};
        getrestime = -1;
        restype = -1;
        pgrtimer = 0;
        getrtimer = 0;
        pgrspeed = pgrspeed2;
        getrspeed = getrspeed2;
        rbasex = -1;
        rbasey = -1;
        sendrtimer = 0;
        sendrspeed = sendrspeed2;
        sendrrange = sendrrange2;
        rbasen = -1;
        name = name2;
        inrange = inrange2;
    }
    void destroyc(GLint dposx, GLint dposy)
    {
        for (long i = 0; i < sx; i++)
        {
            for (long j = 0; j < sy; j++)
            {
                gmap[dposx + i][dposy + j].param[1] = -1;
                gmap[dposx + i][dposy + j].param[2] = 0;
            }
        }
    }
    void stepgo()
    {
        GLint gotime = mgametime - stept;
        if (gotime >= speed)
        {
            stept += gotime;
            destroyc(px, py);
            buildobj(posx, posy, sx, sy, true, thisp, thispn);
            px = posx;
            py = posy;
            steppos = {0.0f, 0.0f};
            step = true;
        }
        else
        {
            steppos = {gopos.first * ((GLfloat)gotime / speed), gopos.second * ((GLfloat)gotime / speed)};
        }
    }
    void go() // update
    {
        bool c = false, f = false;
        GLint nposx = 0, nposy = 0;
        pair<GLint, GLint> m[8] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        stept = glutGet(GLUT_ELAPSED_TIME);
        if (speed == 0)
        {
            act = -1;
            nx = posx;
            ny = posy;
            return;
        }
        if (waypos.count({posx, posy}) != 0)
        {
            act = -1;
            nx = posx;
            ny = posy;
            return;
        }
        waypos.insert({posx, posy});
        if (posx == nx && posy == ny)
        {
            act = -1;
        }
        else
        {
            if (nx < posx)
            {
                swap(m[1], m[3]);
            }
            if (ny < posy)
            {
                swap(m[0], m[2]);
            }
            if (max(nx - posx, posx - nx) > max(ny - posy, posy - ny))
            {
                swap(m[0], m[1]);
            }
            else
            {
                swap(m[2], m[3]);
            }
            f = false;
            for (long i = 0; i < 4; i++)
            {
                GLint d = (m[i].second>0)?(m[i].second+sy-1):(m[i].second);
                GLint e = (m[i].first>0)?(m[i].first+sx-1):(m[i].first);
                c = true;
                if (posx + e >= 0 && posy + d >= 0 && posx + e < gmapx && posy + d < gmapy)
                {
                    if (m[i].second != 0)
                    {
                        for (long j = 0; j < sx && c; j++)
                        {
                            if (gmap[posx + j][posy + d].param[0] != 0 || gmap[posx + j][posy + d].param[1] != -1)
                            {
                                c = false;
                            }
                        }
                    }
                    if (m[i].first != 0)
                    {
                        for (long j = 0; j < sy && c; j++)
                        {
                            if (gmap[posx + e][posy + j].param[0] != 0 || gmap[posx + e][posy + j].param[1] != -1)
                            {
                                c = false;
                            }
                        }
                    }
                    if (m[i].first != 0 && m[i].second != 0 && (gmap[posx + m[i].first + sx * ((m[i].first + 1) / 2)][posy + m[i].second + sy * ((m[i].second + 1) / 2)].param[0] != 0 || gmap[posx + m[i].first + (sx - 1) * ((m[i].first + 1) / 2) + m[i].first][posy + m[i].second + (sy - 1) * ((m[i].second + 1) / 2) + m[i].second].param[1] != -1))
                    {
                        c = false;
                    }
                }
                else
                {
                    c = false;
                }
                if (c)
                {
                    if (posx + m[i].first == pgposx && posy + m[i].second == pgposy)
                    {
                        f = true;
                        c = false;
                    }
                    else
                    {
                        nposx = m[i].first;
                        nposy = m[i].second;
                        gopos = m[i];
                        break;
                    }
                }
            }
            if (c || f)
            {
                px = posx;
                py = posy;
                pposx = posx;
                pposy = posy;
                if (c)
                {
                    pgposx = posx;
                    pgposy = posy;
                    posx += nposx;
                    posy += nposy;
                }
                else
                {
                    swap(posx, pgposx);
                    swap(posy, pgposy);
                    gopos.first = -gopos.first;
                    gopos.second = -gopos.second;
                }
                for (long i = 0; i < sx; i++)
                {
                    for (long j = 0; j < sy; j++)
                    {
                        gmap[posx + i][posy + j].param[1] = thisp;
                        gmap[posx + i][posy + j].param[2] = thispn;
                    }
                }
                step = false;
            }
        }
    }
    bool canattack(GLint &aplayer, GLint &anum);

    bool attack(GLint aplayer, GLint anum, bool c);

    void build();

    void create();

    bool findrbase(GLint &x, GLint &y, GLint &n);

    bool viewres(GLint &x, GLint &y, GLint &num /*resource type*/);

    bool getres(GLint &x, GLint &y, GLint &num /*resource type*/);

    void cstep(GLint player, GLint playern);

    void addcanon(canon a)
    {
        a.p = thisp;
        a.n = thispn;
        weapon.push_back(a);
        for (long i = weapon.size() - 2; i >= 0; i++)
        {
            if (weapon[i].range < weapon[i + 1].range)
            {
                swap(weapon[i], weapon[i + 1]);
            }
            else
            {
                break;
            }
        }
    } // not best

    void draw()
    {
        if (insidec)
        {
            return;
        }
        GLfloat a = rposx - mdrawx, b = rposy - mdrawy;
        GLfloat c = a + msqx * sx, d = b + msqy * sy, e = msqy * sy * 0.1f;
        if (slive < shield[0].a)
        {
            slive = shield[0].a;
        } // ??
        if (slive == 0)
        {
            slive = 1;
        }
        glColor3f(pcolor[thisp].R, pcolor[thisp].G, pcolor[thisp].B);
        square2d(a, b, c, d);
        if (active && thisp == actplayer)
        {
            glColor3f(0.0f, 1.0f, 0.0f);
            squareline(a, b, c, d, 2.0f);
        }
        if ((active && thisp == actplayer) || (gmapposx >= posx && gmapposy >= posy && gmapposx < posx + sx && gmapposy < posy + sy))
        {
            if (buildp < 1.0f)
            {
                glColor3f(0.0f, 0.0f, 0.0f);
                square2d(a, d - 2 * e, c, d);
                glColor3f(0.7f, 0.7f, 0.0f);
                square2d(a, d - 2 * e, a + msqx * sx * buildp, d);
                glColor3f(0.0f, 0.0f, 0.0f);
                squareline(a, d - 2 * e, c, d, 1.0f);
            }
            else
            {
                glColor3f(0.0f, 0.0f, 0.0f);
                square2d(a, d - 2 * e, c, d);
                glColor3f(0.0f, 0.7f, 0.0f);
                square2d(a, d - 2 * e, a + msqx * sx * (shield[0].a / (GLfloat)slive), d);
                glColor3f(0.0f, 0.0f, 0.0f);
                squareline(a, d - 2 * e, c, d, 1.0f);
            }
        }
    }
    ~creature()
    {

    }
};

vector<vector<obj> > gameobj; // player / obj types // modifications

vector<map<GLint, creature> > mapobj; // player / creatures

vector<vector<GLint> > actcreature(playernum);

creature createobj(obj *a, GLint t, GLint x, GLint y, GLint p);

pair<GLint, GLint> spiralfindcplace(GLint x, GLint y, GLint sx, GLint sy, GLint csx, GLint csy) // position x, position y, size x, size y, creature size x, creature size y
{
    GLint a = 0, b = 0; // case num, dist to center
    pair<GLint, GLint> pos = {x, y - 1};
    for (long i = 0; i <= max(gmapx, gmapy) * max(gmapx, gmapy) + 1; i++)
    {
        if (buildobj(pos.first, pos.second, csx, csy, false, -1, -1))
        {
            return pos;
        }
        switch(a)
        {
        case 0:
            pos.first++;
            if (pos.first > x + sx + b)
            {
                a = 1;
            }
            break;
        case 1:
            pos.second++;
            if (pos.second > y + sy + b)
            {
                a = 2;
            }
            break;
        case 2:
            pos.first--;
            if (pos.first < x - b)
            {
                a = 3;
            }
            break;
        case 3:
            if (pos.second < y - b - 1)
            {
                a = 0;
                b++;
            }
            pos.second--;
            break;
        default:
            break;
        }
    }
    return {-1, -1};
}

bool creature::viewres(GLint &x, GLint &y, GLint &num /*resource type*/)
{
    x = -1;
    y = -1;
    //GLint a = posx + sx / 2, b = posy + sy / 2;
    GLint c = 0, d = 0; // position
    GLint e = -1, f = -1; // res dist, res type
    for (long i = -vrange; i <= vrange + sx; i++)
    {
        for (long j = -vrange; j <= vrange + sy; j++)
        {
            c = i + posx;
            d = j + posy;
            if (c >= 0 && d >= 0 && c < gmapx && d < gmapy)
            {
                if (gmap[c][d].param[0] == 2 && (num == -1 || gmap[c][d].rp[0] == num) && gmap[c][d].rp[1] > 0 && ((e == -1) || (e > max(abs(i), abs(j)))))
                {
                    x = c;
                    y = d;
                    e = max(abs(i), abs(j));
                    f = gmap[c][d].rp[0];
                }
            }
        }
    }
    if (x == -1 && y == -1 && f == -1)
    {
        return false;
    }
    num = f;
    return true;
}

bool creature::getres(GLint &x, GLint &y, GLint &num /*resource type*/)
{
    //GLint a = posx + sx / 2, b = posy + sy / 2;
    GLint c = 0, d = 0; // position
    GLint e = 0;
    for (long i = -rrange; i <= rrange + sx; i++)
    {
        for (long j = -rrange; j <= rrange + sy; j++)
        {
            c = posx + i;
            d = posy + j;
            if (c >= 0 && d >= 0 && c < gmapx && d < gmapy)
            {
                if (gmap[c][d].param[0] == 2 && (num == -1 || gmap[c][d].rp[0] == num) && gmap[c][d].rp[1] > 0)
                {
                    x = c;
                    y = d;
                    num = gmap[c][d].rp[0];
                    // add time
                    gminimap[c / (gmapx / gminimapx)][d / (gmapy / gminimapy)].rp[1] -= gmap[c][d].rp[1];
                    e = tplace.add(gmap[c][d].rp[1], gmap[c][d].rp[0], getrtimer, getrspeed);
                    if (e == 0) // destroy resource place
                    {
                        gmap[c][d].rp[0] = -1;
                        gmap[c][d].rp[1] = -1;
                        gmap[c][d].param[0] = 0;
                    }
                    else
                    {
                        gmap[c][d].rp[1] = e;
                        gminimap[c / (gmapx / gminimapx)][d / (gmapy / gminimapy)].rp[1] += e;
                    }
                    setminimapres(c / (gmapx / gminimapx), d / (gmapy / gminimapy));
                    return true;
                }
            }
        }
    }
    return false;
}

bool creature::findrbase(GLint &x, GLint &y, GLint &n)
{
    GLint dist = -1;
    n = -1;
    for (auto it = mapobj[thisp].begin(); it != mapobj[thisp].end(); it++)
    {
        if (it->second.pgrspeed > 0 && it->second.buildp >= 1.0f && (restype == -1 || it->second.tplace.r[restype].second > 0) && (dist == -1 || max(abs(it->second.posx - posx), abs(it->second.posy - posy)) < dist)) // remake with many res types ??
        {
            dist = max(abs(it->second.posx - posx), abs(it->second.posy - posy));
            x = it->second.posx;
            y = it->second.posy;
            n = it->first;
        }
    }
    if (n == -1)
    {
        return false;
    }
    return true;
}

void creature::cstep(GLint player, GLint playern)
{
    bool b = true;
    GLint c = 0;
    creature *d;
    GLint e = 0;
    tplace.step();
    if (pact != act)
    {
        waypos.clear();
        pgposx = -1;
        pgposy = -1;
    }
    pact = act;
    if (buildp >= 1.0f)
    {
        if (step)
        {
            GLint aplayer = -1, anum = -1;
            if (insidec)
            {
                if (outsidec)
                {
                    destroyc(posx, posy);
                }
                act = -1;
                b = false;
            }
            else
            {
                if (!outsidec)
                {
                    nx = posx;
                    ny = posy;
                    px = posx;
                    py = posy;
                    pposx = -1;
                    pposy = -1;
                    pgposx = -1;
                    pgposy = -1;
                    buildobj(posx, posy, sx, sy, true, thisp, thispn);
                }
            }
            outsidec = !insidec;
            switch(act)
            {
            case -1:
                nx = posx;
                ny = posy;
                pgposx = -1;
                pgposy = -1;
                getrestime = -1;
                waypos.clear();
                break;
            case 0: // go
                go();
                break;
            case 1: // attack
                if (weapon.size() == 0)
                {
                    act = -1;
                    break;
                }
                if (attack(attackplayer, attackobj, true))
                {
                    b = false;
                    nx = posx;
                    ny = posy;
                }
                else
                {
                    go();
                }
                break;
            case 2: // build
                // add move and create
                build();
                break;
            case 3: // create
                create();
                break;
            case 4: // get resources
                if ((posx == rpos.first && posy == rpos.second) || rpos.first == -1 || rpos.second == -1)
                {
                    if (!viewres(rpos.first, rpos.second, restype))
                    {
                        restype = -1;
                        if (!viewres(rpos.first, rpos.second, restype));
                        {
                            act = -1;
                            break;
                        }
                    }
                }
                if (getres(rpos.first, rpos.second, restype))
                {
                    nx = posx;
                    ny = posy;
                    if (tplace.resfull(restype))
                    {
                        act = 5;
                    }
                }
                else
                {
                    nx = rpos.first;
                    ny = rpos.second;
                    go();
                    if (act == -1)
                    {
                        restype = -1;
                        if (getres(rpos.first, rpos.second, restype))
                        {
                            nx = posx;
                            ny = posy;
                            act = 4;
                            if (tplace.resfull(restype))
                            {
                                act = 5;
                            }
                        }
                        else
                        {
                            if (!tplace.resempty())
                            {
                                act = 5;
                            }
                            else
                            {
                                act = -1;
                            }
                        }
                    }
                }
                break;
            case 5: // return to base with resources
                if (pgrspeed > 0)
                {
                    pgrtimer = mgametime;
                    act = -1;
                }
                else
                {
                    if (sendrspeed == 0)
                    {
                        act = -1;
                        break;
                    }
                    if (findrbase(nx, ny, rbasen))
                    {
                        act = 6;
                    }
                    else
                    {
                        act = -1;
                    }
                }
                break;
            case 6: // send resources
                if (mapobj[thisp].count(rbasen) == 0)
                {
                    nx = posx;
                    ny = posy;
                    act = 5;
                    break;
                }
                d = &mapobj[thisp][rbasen];
                if (max(abs(d->posx - posx), abs(d->posy - posy)) <= sendrrange)
                {
                    //send res
                    c = (mgametime - sendrtimer) / sendrspeed;
                    sendrtimer = mgametime - ((mgametime - sendrtimer) % sendrspeed);
                    for (long i = 0; i < tplace.r.size(); i++)
                    {
                        if (d->tplace.r[i].second > 0)
                        {
                            e = min(c, min(tplace.r[i].first.n, d->tplace.r[i].second - d->tplace.r[i].first.n));
                            d->tplace.r[i].first.n += e;
                            tplace.r[i].first.n -= e;
                            c -= e;
                            if (c <= 0)
                            {
                                break;
                            }
                        }
                    }
                    if (tplace.resempty())
                    {
                        sendrtimer = mgametime;
                        act = 4;
                    }
                }
                else
                {
                    nx = d->posx;
                    ny = d->posy;
                    go();
                }
                break;
            case 7: // go inside creature
                break;
            default:
                break;
            }
            if (b)
            {
                if (canattack(aplayer, anum))
                {
                    attack(aplayer, anum, false);
                }
            }
            if (pgrspeed > 0)
            {
                if (!tplace.resempty())
                {
                    c = (mgametime - pgrtimer) / pgrspeed;
                    pgrtimer = mgametime - ((mgametime - pgrtimer) % pgrspeed);
                    for (long i = 0; i < tplace.r.size(); i++)
                    {
                        presources[thisp][i].n += min(tplace.r[i].first.n, c);
                        tplace.r[i].first.n = max(0, tplace.r[i].first.n - c);
                        c -= tplace.r[i].first.n;
                        c = max(0, c);
                        if (c <= 0)
                        {
                            break;
                        }
                    }
                    if (c > 0)
                    {
                        pgrtimer = mgametime;
                    }
                    c = 0;
                }
                else
                {
                    pgrtimer = mgametime;
                }
            }
            if (mgametime - getrtimer > getrspeed || act != 4)
            {
                getrtimer = mgametime;
            }
            if (mgametime - getrtimer > sendrspeed || act != 6)
            {
                sendrtimer = mgametime;
            }
        }
        else
        {
            stepgo();
        }
        if (!step)
        {
            rposx = px + steppos.first;
            rposy = py + steppos.second;
        }
        else
        {
            rposx = posx;
            rposy = posy;
        }
    }
    else
    {
        act = -1;
    }
}


class astruct
{
public:
    GLint n, p; // id, num, player
    astruct(GLint n2 = 0, GLint p2 = 0)
    {
        n = n2;
        p = p2;
    }
    ~astruct()
    {

    }
};

class athing
{
public:
    astruct from;
    astruct to;
    bool animattack;
    GLint t, time, atime, r; // type, time, animation after attack time, range of attack
    GLfloat s, posx, posy, nposx, nposy, a; // speed, position x, position y, new posx, new posy, attack
    athing(astruct from2, astruct to2, GLfloat s2 = 0.0f, GLint t2 = 0, GLfloat a2 = 0, GLint atime2 = 0, GLint r2 = 0)
    {
        s = s2;
        t = t2;
        time = mgametime;
        a = a2;
        atime = atime2;
        r = r2;
        from = from2;
        to = to2;
        animattack = false;
        posx = mapobj[from.p][from.n].rposx + mapobj[from.p][from.n].sx / 2.0f;
        posy = mapobj[from.p][from.n].rposy + mapobj[from.p][from.n].sy / 2.0f;
        nposx = mapobj[to.p][to.n].rposx + mapobj[to.p][to.n].sx / 2.0f;
        nposy = mapobj[to.p][to.n].rposy + mapobj[to.p][to.n].sy / 2.0f;
    }
    bool go()
    {
        GLfloat dtime = (mgametime - time) / (GLfloat)1000; // in seconds
        GLfloat dist = 0, distx = 0, disty = 0;
        bool b = false;
        time = mgametime;
        if (mapobj[from.p].count(from.n) != 0 && mapobj[to.p].count(to.n) != 0)
        {
            if (animattack)
            {
                if (t == 0)
                {
                    posx = mapobj[from.p][from.n].rposx + mapobj[from.p][from.n].sx / 2.0f;
                    posy = mapobj[from.p][from.n].rposy + mapobj[from.p][from.n].sy / 2.0f;
                }
                if (t == 0 || t == 1)
                {
                    nposx = mapobj[to.p][to.n].posx + mapobj[to.p][to.n].sx / 2.0f;
                    nposy = mapobj[to.p][to.n].posy + mapobj[to.p][to.n].sy / 2.0f;
                }
                if (t == 1 || t == 2)
                {
                    posx = nposx;
                    posy = nposy;
                }
                atime -= dtime * 1000;
                if (atime <= 0)
                {
                    atime = 0;
                    b = true;
                }
            }
            else
            {
                // t: 0 - laser, 1 - canon, 2 -
                if (t == 0 || t == 1)
                {
                    nposx = mapobj[to.p][to.n].rposx + mapobj[to.p][to.n].sy / (GLfloat)2;
                    nposy = mapobj[to.p][to.n].rposy + mapobj[to.p][to.n].sy / (GLfloat)2;
                }
                if (t == 1 || t == 2)
                {
                    distx = nposx - posx;
                    disty = nposy - posy;
                    dist = sqrt(distx * distx + disty * disty);
                }
                if (dist <= s * dtime || t == 0)
                {
                    animattack = true;
                    if (t != 0)
                    {
                        posx = nposx;
                        posy = nposy;
                    }
                    if (t == 0 || t == 1)
                    {
                        mapobj[to.p][to.n].shield[0].a -= a;
                        if (mapobj[to.p][to.n].shield[0].a < 0)
                        {
                            mapobj[to.p][to.n].shield[0].a = 0;
                        }
                    }
                    if (t == 2)
                    {
                        for (long i = posx - r + 1; i <= posx + r - 1; i++)
                        {
                            if (i >= 0 && i < gmapx)
                            {
                                for (long j = posy - r + 1; j <= posy + r - 1; j++)
                                {
                                    if (j >= 0 && j < gmapy && gmap[i][j].param[1] >= 0)
                                    {
                                        mapobj[gmap[i][j].param[1]][gmap[i][j].param[2]].shield[0].a -= min((a * ((r - max(max(i - (GLint)posx, (GLint)posx - i), max(j - (GLint)posy, (GLint)posy - j))) / (GLfloat)r)), mapobj[gmap[i][j].param[1]][gmap[i][j].param[2]].shield[0].a);
                                    }
                                }
                            }
                        }
                        // dist - -attack ???
                    }
                }
                else
                {
                    posx += (distx / dist) * s * dtime;
                    posy += (disty / dist) * s * dtime;
                }
            }
        }
        else
        {
            b = true;
        }
        return b;
    }
    void draw()
    {
        if (t == 0)
        {
            glColor3f(0.5f, 0.5f, 1.0f);
            line(posx - mdrawx, posy - mdrawy, nposx - mdrawx, nposy - mdrawy, 4.0f);
        }
        if (t == 1)
        {
            glColor3f(0.4f, 0.4f, 0.4f);
            square2d(posx - 0.2f - mdrawx, posy - 0.2f - mdrawy, posx + 0.2f - mdrawx, posy + 0.2f - mdrawy);
        }
        if (t == 2)
        {
            glColor3f(0.2f, 0.2f, 0.2f);
            square2d(posx - 0.3f - mdrawx, posy - 0.3f - mdrawy, posx + 0.3f - mdrawx, posy + 0.3f - mdrawy);
        }
    }
    ~athing()
    {

    }
};

vector<vector<athing> > attackthing(playernum);

bool canon::attackobj(GLint player, GLint obj, bool aobj) // aobj - attack / without attack
{
    bool b = false;
    creature *c = &mapobj[player][obj];
    if (getdist(mapobj[p][n].rposx + (GLfloat)mapobj[p][n].sx / 2, mapobj[p][n].rposy + (GLfloat)mapobj[p][n].sy / 2, (c->rposx) + (GLfloat)(c->sx) / 2, (c->rposy) + (GLfloat)(c->sy) / 2))
    {
        b = true; // ??
        if (time + timer <= mgametime)
        {
            if (aobj)
            {
                timer = mgametime;
                astruct t(obj, player);
                astruct f(n, p);
                athing d(f, t, s, atype, attack, 1000, r);
                attackthing[p].push_back(d);
                //p->shield[0].a -= attack;
                //if (p->shield[0].a < 0)
                //{
                //    p->shield[0].a = 0;
                //}
            }
        }
    }
    return b;
}

bool creature::canattack(GLint &aplayer, GLint &anum)
{
    bool b = false;
    if (weapon.size() > 0)
    {
        for (long i = 0; i < mapobj.size() && b == false; i++)
        {
            if (i != thisp)
            {
                for (map<GLint, creature>::iterator it = mapobj[i].begin(); it != mapobj[i].end(); it++)
                {
                    if(weapon[0].attackobj(thisp, thispn, false) && (it->second).shield[0].a > 0)
                    {
                        b = true;
                        aplayer = i;
                        anum = it->first;
                        break;
                    }
                }
            }
        }
    }
    return b;
} // not best

bool creature::attack(GLint aplayer, GLint anum, bool c)
{
    creature *a = &mapobj[aplayer][anum];
    bool b = false;
    if (aplayer != -1)
    {
        if (mapobj[aplayer].count(anum) == 0 || a->shield[0].a <= 0)
        {
            if (c)
            {
                act = -1;
                b = true;
            }
            return b;
        }
        for (long i = 0; i < weapon.size(); i++)
        {
            if (weapon[i].attackobj(aplayer, anum, true))
            {
                b = true;
            }
        }
        if (!b && weapon.size() > 0 && c)
        {
            nx = a->posx;
            ny = a->posy;
        }
    }
    return b;
}

void creature::build()
{
    GLint a = -1, b = 0, c = 0;
    creature *d, *e;
    if (mapobj[thisp][bcreature].buildp < 1.0f)
    {
        if (mapobj[thisp].count(bcreature) != 0)
        {
            d = &mapobj[thisp][bcreature];
            e = &mapobj[thisp][thispn];
            b = abs(d->rposx + d->sx / 2.0f - e->rposx - e->sx / 2.0f);
            c = abs(d->rposy + d->sx / 2.0f - e->rposy - e->sy / 2.0f);
            for (long i = 0; i < btype.size(); i++)
            {
                if (btype[i].t == mapobj[thisp][bcreature].t)
                {
                    a = i;
                    break;
                }
            }
            if (a != -1 && b <= brange && c <= brange)
            {
                nx = posx;
                ny = posy;
                if (mgametime >= btime + btimer)
                {
                    btime = mgametime;
                    mapobj[thisp][bcreature].buildp += (GLfloat)1 / (GLfloat)max(bspeed * btype[a].d, (GLfloat)1);
                    if (mapobj[thisp][bcreature].buildp >= 1.0f)
                    {
                        mapobj[thisp][bcreature].buildp = 1.2f; // ??
                    }
                }
            }
            else
            {
                nx = mapobj[thisp][bcreature].posx;
                ny = mapobj[thisp][bcreature].posy;
                go();
            }
        }
        else
        {
            nx = posx;
            ny = posy;
            act = -1;
        }
    }
    else
    {
        act = -1;
    }
} // add build range

void creature::create()
{
    bobj *d = &btype[buildt];
    obj *e = &gameobj[thisp][d->t];
    bool f = true;
    GLfloat g = abs(rposx + sx / 2.0f - bmapx);
    GLfloat h = abs(rposy + sy / 2.0f - bmapy);
    for (long i = 0; i < resourcenum; i++)
    {
        if (presources[thisp][i].n <= d->r[i].n)
        {
            f = false;
        }
    }
    if (buildobj(bmapx - ceil(e->sx / 2.0f) + 1, bmapy - ceil(e->sy / 2.0f) + 1, 1, 1, false) && f)
    {
        if (g <= brange && h <= brange)
        {
            nx = posx;
            ny = posy;
            creature c = createobj(e, d->t, bmapx - ceil(e->sx / 2.0f) + 1, bmapy - ceil(e->sy / 2.0f) + 1, thisp);
            for (long i = 0; i < resourcenum; i++)
            {
                presources[thisp][i].n -= d->r[i].n;
            }
            buildobj(bmapx - ceil(e->sx / 2.0f) + 1, bmapy - ceil(e->sy / 2.0f) + 1, e->sx, e->sy, true, thisp, worldid[thisp]);
            mapobj[thisp][worldid[thisp]++] = c;
            mapobj[thisp][thispn].bcreature = worldid[thisp] - 1;
            mapobj[thisp][thispn].act = 2;
        }
        else
        {
            nx = bmapx;
            ny = bmapy;
            go();
        }
    }
    else
    {
        nx = posx;
        ny = posy;
        act = -1;
    }
}

void cargo::step()
{
    for (long i = 0; i < t.size(); i++)
    {
        if (mapobj[t[i].first].count(t[i].second.first) == 0)
        {
            w -= t[i].second.second;
            if (i != t.size() - 1)
            {
                swap(t[i], t[t.size() - 1]);
            }
            t.pop_back();
            i--;
        }
    }
}

void initmap()
{
    gameobj.clear();
    gameobj.resize(playernum);
    mapobj.clear();
    mapobj.resize(playernum);
    gmap.clear();
    gmap.resize(gmapx, vector<melem> (gmapy));
    gminimap.clear();
    gminimap.resize(gminimapx + 1, vector<melem> (gminimapy + 1));
    pcolor.clear();
    pcolor.resize(playernum);
    for (long i = 0; i < gmapx; i++)
    {
        for (long j = 0; j < gmapy; j++)
        {
            for (long k = 0; k < gmapparamnum; k++)
            {
                gmap[i][j].param[k] = 0;//(i + gmapx * j) % 4;
            }
            gmap[i][j].param[1] = -1;
        }
    }
    gmap[4][4].param[0] = 2;
    gmap[4][4].rp[0] = 0;
    gmap[4][4].rp[1] = 20;
    for (long i = 0; i < gmapx + gmapx / gminimapx - (gmapx / gminimapx > 0?1:0); i += gmapx / gminimapx)
    {
        for (long j = 0; j < gmapy + gmapy / gminimapy - (gmapy / gminimapy > 0?1:0); j += gmapy / gminimapy)
        {
            for (long k = 0; k < gmapx / gminimapx; k++)
            {
                if (i + k >= 0 && i + k < gmapx)
                {
                    for (long p = 0; p < gmapy / gminimapy; p++)
                    {
                        if (j + p >= 0 && j + p < gmapy)
                        {
                            if (gmap[i + k][j + p].param[0] == 2)
                            {
                                gminimap[i / (gmapx / gminimapx)][j / (gmapy / gminimapy)].rp[1] += gmap[i + k][j + p].rp[1];
                            }
                        }
                    }
                }
                setminimapres(i, j);
            }
        }
    }
}

void initGL()
{
	glClearColor (1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth (100.0f);
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_TEXTURE_2D);
	glDepthFunc (GL_LEQUAL);
	glShadeModel (GL_SMOOTH);
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/*
void addmodel(string s)
{
	GLint n = 0;
	GLint h = 0;
	GLint b = 0;
	ifstream add;
	add.open(s);
	add >> n >> h;
	model a(n, h);
	for (GLint i = 0; i < n * h; i++)
	{
		for (GLint j = 0 ; j < n; j++)
		{
			for (GLint k = 0; k < n; k++)
			{
				add >> b;
				if (b == 1)
				{
					a.m[j][k][i].s = true;
				}
			}
		}
	}
	for (GLint i = 0; i < n * h; i++)
	{
		for (GLint j = 0 ; j < n; j++)
		{
			for (GLint k = 0; k < n; k++)
			{
				add >> a.m[j][k][i].r;
				add >> a.m[j][k][i].g;
				add >> a.m[j][k][i].b;
				add >> a.m[j][k][i].a;
			}
		}
	}
	for (GLint i = 0; i < n * h; i++)
	{
		for (GLint j = 0 ; j < n; j++)
		{
			for (GLint k = 0; k < n; k++)
			{
				add >> a.m[j][k][i].t;
			}
		}
	}
	cmodel.push_back(a);
	add.close();
} // ???

void drawmodel(GLint num, bool d)
{
	GLfloat a = 0, b = 0, c = 0;
	glBindTexture (GL_TEXTURE_2D, texture[1]);
	for (GLint i = 0; i < cmodel[num].n * cmodel[num].h; i++)
	{
		for (GLint j = 0; j < cmodel[num].n; j++)
		{
			for (GLint k = 0; k < cmodel[num].n; k++)
			{
				if (cmodel[num].m[j][k][i].s)
				{
					if (j == 0 || !cmodel[num].m[j - 1][k][i].s)
					{
						if (d)
						{
							square(a, b, c, a, b + 1.0f * squarey / cmodel[num].n, c + 1.0f * squarez / cmodel[num].n, cmodel[num].m[j][k][i].r - bshadow, cmodel[num].m[j][k][i].g - bshadow, cmodel[num].m[j][k][i].b - bshadow, cmodel[num].m[j][k][i].a);
						}
						else
						{
							glColor4f(cmodel[num].m[j][k][i].r, cmodel[num].m[j][k][i].g, cmodel[num].m[j][k][i].b, cmodel[num].m[j][k][i].a);
							tsquare(cmodel[num].m[j][k][i].t, a, b, c, a, b + 1.0f * squarey / cmodel[num].n, c + 1.0f * squarez / cmodel[num].n);
						}
					}
					if (k == 0 || !cmodel[num].m[j][k - 1][i].s)
					{
						if (d)
						{
							square(a, b, c, a + 1.0f * squarex / cmodel[num].n, b, c + 1.0f * squarez / cmodel[num].n, cmodel[num].m[j][k][i].r - bshadow, cmodel[num].m[j][k][i].g - bshadow, cmodel[num].m[j][k][i].b - bshadow, cmodel[num].m[j][k][i].a);
						}
						else
						{
							tsquare(cmodel[num].m[j][k][i].t, a, b, c, a + 1.0f * squarex / cmodel[num].n, b, c + 1.0f * squarez / cmodel[num].n);
						}
					}
					if (i == 0 || !cmodel[num].m[j][k][i - 1].s)
					{
						if (d)
						{
							square(a, b, c, a + 1.0f * squarex / cmodel[num].n, b + 1.0f * squarey / cmodel[num].n, c, cmodel[num].m[j][k][i].r, cmodel[num].m[j][k][i].g, cmodel[num].m[j][k][i].b, cmodel[num].m[j][k][i].a);
						}
						else
						{
							tsquare(cmodel[num].m[j][k][i].t, a, b, c, a + 1.0f * squarex / cmodel[num].n, b + 1.0f * squarey / cmodel[num].n, c);
						}
					}
					c +=  1.0f * squarez / cmodel[num].n;
					a +=  1.0f * squarex / cmodel[num].n;
					b +=  1.0f * squarey / cmodel[num].n;
					if (j + 1 == cmodel[num].n || !cmodel[num].m[j + 1][k][i].s)
					{
						if (d)
						{
							square(a, b, c, a, b - 1.0f * squarey / cmodel[num].n, c - 1.0f * squarez / cmodel[num].n, cmodel[num].m[j][k][i].r - bshadow, cmodel[num].m[j][k][i].g - bshadow, cmodel[num].m[j][k][i].b - bshadow, cmodel[num].m[j][k][i].a);
						}
						else
						{
							tsquare(cmodel[num].m[j][k][i].t, a, b, c, a, b - 1.0f * squarey / cmodel[num].n, c - 1.0f * squarez / cmodel[num].n);
						}
					}
					if (k + 1 == cmodel[num].n || !cmodel[num].m[j][k + 1][i].s)
					{
						if (d)
						{
							square(a, b, c, a - 1.0f * squarex / cmodel[num].n, b, c - 1.0f * squarez / cmodel[num].n, cmodel[num].m[j][k][i].r - bshadow, cmodel[num].m[j][k][i].g - bshadow, cmodel[num].m[j][k][i].b - bshadow, cmodel[num].m[j][k][i].a);
						}
						else
						{
							tsquare(cmodel[num].m[j][k][i].t, a, b, c, a - 1.0f * squarex / cmodel[num].n, b, c - 1.0f * squarez / cmodel[num].n);
						}
					}
					if (i + 1 == cmodel[num].n * cmodel[num].h || !cmodel[num].m[j][k][i + 1].s)
					{
						if (d)
						{
							square(a, b, c, a - 1.0f * squarex / cmodel[num].n, b - 1.0f * squarey / cmodel[num].n, c, cmodel[num].m[j][k][i].r, cmodel[num].m[j][k][i].g, cmodel[num].m[j][k][i].b, cmodel[num].m[j][k][i].a);
						}
						else
						{
							tsquare(cmodel[num].m[j][k][i].t, a, b, c, a - 1.0f * squarex / cmodel[num].n, b - 1.0f * squarey / cmodel[num].n, c);
						}
					}
					c -=  1.0f * squarez / cmodel[num].n;
					a -=  1.0f * squarex / cmodel[num].n;
					//b += 1.0f * squarey / cmodel[num].n;
				}
				else
				{
					b += 1.0f * squarey / cmodel[num].n;
				}
			}
			b -= 1.0f;
			a += 1.0f * squarex / cmodel[num].n;
		}
		a -= 1.0f;
		c += 1.0f * squarez / cmodel[num].n;
	}
	c -= 1.0f;
}
*/

void loadtexture()
{
	GLint i = 0;
	char *a[texturenum] = {};
	for (i = 0; i < texturenum; i++)
	{
		int imagepos = 0, imagew = 0, imageh = 0, imagesize = 0;
		unsigned char imagedata_s[54];
		unsigned char *imagedata;
		FILE *imagefile = fopen(a[i], "rb");
		fread (imagedata_s, sizeof(char), 54, imagefile);
		imagew = *(int*)&(imagedata_s[0x12]);
		imageh = *(int*)&(imagedata_s[0x16]);
		imagesize = *(int*)&(imagedata_s[0x22]);
		imagepos = *(int*)&(imagedata_s[0x0A]);
		if (imagesize == 0)
		{
			imagesize = imagew * imageh * 4;// RGBA
		}
		if (imagepos == 0)
		{
			imagepos = 54;
		}
		imagedata = (unsigned char*)malloc(imagesize * sizeof(unsigned char));
		fread (imagedata, sizeof(char), imagesize, imagefile);
		for (GLint i = 0; (i + 2) < imagesize; i+=4)
		{
			swap(imagedata[i], imagedata[i + 2]);
		}
		fclose(imagefile);
		glGenTextures(1, &texture[i]);
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, imagew, imageh, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagedata);
		free(imagedata);
    }
}

void text(string s)
{
	GLint i = 0;
	for (i = 0; i < s.size(); i++)
	{
		if (s[i] == '\n')
		{
			startstring[1] -= 0.007f;
			glRasterPos3f(startstring[0], startstring[1], startstring[2]);
		}
		else
		{
			glutBitmapCharacter (GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
		}
	}
}

void text(GLint s)
{
	GLint i = 0, a = 0, b = 100000LL;
	char c;
	bool d = false;
	for (i = inttextmax; i > 0; i--)
	{
		a = (s / b) % 10;
		c = '0' + a;
		if (a > 0 || d)
		{
			d = true;
			glutBitmapCharacter (GLUT_BITMAP_TIMES_ROMAN_24, c);
		}
		b /= 10;
	}
	if (d == false)
	{
		glutBitmapCharacter (GLUT_BITMAP_TIMES_ROMAN_24, '0');
	}
}

void text(string s, GLfloat tx, GLfloat ty, GLfloat tz)
{
	glRasterPos3f (tx, ty, tz + 0.0001f);
	startstring[0] = tx;
	startstring[1] = ty;
	startstring[2] = tz + 0.0001f;
	text(s);
}

void text(GLint s, GLfloat tx, GLfloat ty, GLfloat tz)
{
	glRasterPos3f (tx, ty, tz + 0.0001f);
	startstring[0] = tx;
	startstring[1] = ty;
	startstring[2] = tz + 0.0001f;
	text(s);
}

void searchmpos(GLfloat mpx, GLfloat mpy, GLfloat *mx, GLfloat *my)
{
	GLfloat mp2[2] = {mpx, mpy};
	if (wp[0] / 8.0f > wp[1] / 6.0f)
	{
		mp2[0] -= (wp[0] - wp[1] / 6.0f * 8.0f) * 0.5f;
		if (mp2[0] < 0)
		{
			mp2[0] = 0;
		}
		if (mp2[0] > wp[0] - (wp[0] - wp[1] / 6.0f * 8.0f) * 0.5)
		{
			mp2[0] = wp[0] - (wp[0] - wp[1] / 6.0f * 8.0f) * 0.5;
		}
		*mx = mp2[0] / (wp[1] / 6.0f * 8.0f) * 0.2f - 0.01f;
		*my = -mp2[1] / wp[1] * 0.148f + 0.146f;
	}
	else
	{
		if (wp[0] / 8.0f < wp[1] / 6.0f)
		{
			mp2[1] -= (wp[1] - wp[0] / 8.0f * 6.0f) * 0.5f;
			if (mp2[1] < 0)
			{
				mp2[1] = 0;
			}
			if (mp2[1] > wp[1] - (wp[1] - wp[0] / 8.0f * 6.0f) * 0.5f)
			{
				mp2[1] = wp[1] - (wp[1] - wp[0] / 8.0f * 6.0f) * 0.5f;
			}
			*mx = mp2[0] / wp[0] * 0.2f - 0.01f;
			*my = -mp2[1] / (wp[0]  / 8.0f * 6.0f) * 0.148f + 0.146f;
		}
		else
		{
			*mx = mp2[0] / wp[0] * 0.2f - 0.01f;
			*my = -mp2[1] / wp[1] * 0.148f + 0.146f;
		}
	}
}

void bsearchmpos(GLint mpx, GLint mpy, GLfloat *mx, GLfloat *my)
{
    searchmpos(mpx, mpy, mx, my);
    *mx -= 0.0075f;
    *my -= 0.0125f;
    *mx *= 200.0f;
    *my *= 200.0f;
	/*GLfloat mp2[2] = {mp[0], mp[1]};
	if (wp[0] / 8.0f > wp[1] / 6.0f)
	{
		mp2[0] -= (wp[0] - wp[1] / 6.0f * 8.0f) * 0.5f;
		if (mp2[0] < 0)
		{
			mp2[0] = 0;
		}
		if (mp2[0] > wp[0] - (wp[0] - wp[1] / 6.0f * 8.0f) * 0.5)
		{
			mp2[0] = wp[0] - (wp[0] - wp[1] / 6.0f * 8.0f) * 0.5;
		}
		*mx = (mp2[0] / (wp[1] / 6.0f * 8.0f) * 0.2f - 0.1f) / 0.18f * 33.0f;
		*my = (-mp2[1] / wp[1] * 0.148f + 0.074f) / 0.18f * 33.0f;
	}
	else
	{
		if (wp[0] / 8.0f < wp[1] / 6.0f)
		{
			mp2[1] -= (wp[1] - wp[0] / 8.0f * 6.0f) * 0.5f;
			if (mp2[1] < 0)
			{
				mp2[1] = 0;
			}
			if (mp2[1] > wp[1] - (wp[1] - wp[0] / 8.0f * 6.0f) * 0.5f)
			{
				mp2[1] = wp[1] - (wp[1] - wp[0] / 8.0f * 6.0f) * 0.5f;
			}
			*mx = (mp2[0] / wp[0] * 0.2f - 0.1f) / 0.18f * 33.0f;
			*my = (-mp2[1] / (wp[0] / 8.0f * 6.0f) * 0.148f + 0.074f) / 0.18f * 33.0f;
		}
		else
		{
			*mx = (mp2[0] / wp[0] * 0.2f - 0.1f) / 0.18f * 33.0f;
			*my = (-mp2[1] / wp[1] * 0.148f + 0.074f) / 0.18f * 33.0f;
		}
	}*/
}

void settime()
{
	if (time(NULL) > dtime)
	{
		dtime = time(NULL);
		addtime = true;
		dtime = time(NULL);
	}
}

void breaktime()
{
	addtime = false;
}

creature createobj(obj *e, GLint t, GLint x, GLint y, GLint p) // obj, type, position x, position y, player
{
    creature c(e->sx, e->sy, x, y, e->sm, t, p, worldid[p], e->speed,
                0.0f, e->sh[0].a, e->sh[0].a, e->bspeed, e->weight, e->vrange, e->brange, e->rrange, e->pgrspeed, e->getrspeed, e->sendrspeed, e->sendrrange, e->inrange, e->name);
    c.tplace = e->tplace;
    for (long i = 0; i < e->w.size(); i++)
    {
        c.addcanon(e->w[i]);
    }
    for (long i = 0; i < shsize; i++)
    {
        c.shield[i] = e->sh[i];
    }
    for (long i = 0; i < e->bt.size(); i++)
    {
        c.btype.push_back(e->bt[i]);
    }
    return c;
}

void fileinit(string filename)
{
    ifstream ininit;
    ifstream rinit;
    GLint n = 0, m = 0;
    GLint x = 0, y = 0;
    string r;
    GLint a;
    ininit.open(filename);
    ininit >> playernum;
    obj b;
    canon c;
    obj *d;
    bobj e;
    creature f;
    for (long i = 0; i < playernum; i++)
    {
        for (long j = 0; j < resourcenum; j++)
        {
            ininit >> presources[i][j].n;
        }
        cin >> r;
        rinit.open(r);
        rinit >> n;
        for (long j = 0; j < n; j++)
        {
            // input obj
            rinit >> b.name;
            rinit >> b.brange >> b.bspeed >> b.getresbase >> b.getrspeed >> b.pgrspeed >> b.rrange >> b.sendrrange >> b.sendrspeed >> b.vrange >> b.inrange;
            rinit >> b.sm >> b.speed >> b.sx >> b.sy >> b.weight;
            rinit >> m;
            for (long k = 0; k < m; k++)
            {
                rinit >> e.t >> e.d;
                rinit >> a;
                if (a == 0)
                {
                    e.in = false;
                }
                else
                {
                    e.in = true;
                }
                for (long p = 0; p < e.r.size(); p++)
                {
                    rinit >> e.r[p].n;
                }
                b.bt.push_back(e);
            }
            for (long k = 0; k < b.sh.size(); k++)
            {
                rinit >> b.sh[k].a;
            }
            rinit >> b.tplace.mw;
            for (long k = 0; k < b.tplace.r.size(); k++)
            {
                rinit >> b.tplace.r[k].second;
            }
            rinit >> m;
            for (long k = 0; k < m; k++)
            {
                rinit >> c.attack >> c.atype >> c.range;
                rinit >> c.type >> c.r >> c.s >> c.time;
                b.w.push_back(c);
            }
            gameobj[i].push_back(b);
        }
        rinit.close();
        ininit >> n;
        for (long j = 0; j < n; j++)
        {
            ininit >> a;
            ininit >> x >> y;
            d = &gameobj[i][a];
            buildobj(x, y, d->sx, d->sy, true, i, worldid[i]);
            f = createobj(d, a, x, y, i);
            f.buildp = 1.2f; // ??
            mapobj[i][worldid[i]++] = f;
        }
    }
    ininit.close();
}

void init()
{
    creature a(2, 2, 0, 0, 0, 0, 0, 0, 200, 1.0f, 100, 100, 1.0f, 0, 100, 5, 2, 0, 500, 500, 3, 0);
    canon b(0, 2000, 5, 10, 2, 0, 0, 2.0f, 2);
    obj c(3, 3, 100, 0, 200, 1.0f, 0, 0, 5, 0, 1000, 0, 0, 0, 0);
    a.tplace.r[0].second = 10;
    c.tplace.r[0].second = 10;
    bobj d;
    d.d = 10;
    d.r[0] = 10;
    presources[0][0] = 25;
    gameobj.resize(playernum);
    mapobj.resize(playernum);
    pcolor.resize(playernum);
    buildtype.resize(playernum, -1);
    gameobj[0].push_back(c);
    pcolor[0].R = 0.7f;
    pcolor[1].B = 0.7f;
    a.btype.push_back(d);
    mapobj[0][worldid[0]++] = a;
    for (long i = 0; i < 2; i++)
    {
        for (long j = 0; j < 2; j++)
        {
            gmap[i][j].param[1] = 0;
            gmap[i][j].param[2] = 0;
        }
    }
    mapobj[0][worldid[0] - 1].addcanon(b);
    a.btype.clear();
    a.posx = 2.0f;
    a.posy = 2.0f;
    a.rposx = 2.0f;
    a.rposy = 2.0f;
    a.sx = 1;
    a.sy = 1;
    a.thisp = 1;
    mapobj[1][worldid[1]++] = a;
    for (long i = 2; i < 3; i++)
    {
        for (long j = 2; j < 3; j++)
        {
            gmap[i][j].param[1] = 1;
            gmap[i][j].param[2] = 0;
        }
    }
}

void creaturestep()
{
    creature *a;
    for (long i = 0; i < playernum; i++)
    {
        for (long j = 0; j < mapobj[i].size(); j++)
        {
            mapobj[i][j].cstep(i, j);
            if (mapobj[i][j].shield[0].a <= 0)
            {
                a = &mapobj[i][j];
                for (long k = a->posx; k < (a->posx + a->sx + 1) && k < gmap.size(); k++)
                {
                    for (long r = a->posy; r < (a->posx + a->sx + 1) && r < gmap[k].size(); r++)
                    {
                        if (gmap[k][r].param[1] == i && gmap[k][r].param[2] == j)
                        {
                            gmap[k][r].param[1] = -1;
                            gmap[k][r].param[2] = 0;
                        }
                    }
                }
                mapobj[i].erase(j);
            }
        }
    }
}

pair<GLfloat, GLfloat> findcpos(creature a, GLfloat x, GLfloat y) // find creature pos
{
    pair<GLfloat, GLfloat> b = {x, y};
    if (x >= a.rposx + a.sx)
    {
        b.first -= a.sx - 1;
    }
    else
    {
        if (x > a.rposx)
        {
           b.first = a.rposx;
        }
    }
    if (y >= a.rposy + a.sy)
    {
        b.second -= a.sy - 1;
    }
    else
    {
        if (y > a.rposy)
        {
            b.second = a.rposy;
        }
    }
    return b;
}

void mouseactclear() // clear actcreature[actplayer] by mouse click
{
    if (mb[1].second)
    {
        for (long i = 0; i < actcreature[actplayer].size(); i++)
        {
            mapobj[actplayer][actcreature[actplayer][i]].active = false;
        }
        actcreature[actplayer].clear();
        buildnum[actplayer] = 0;
        buildtype[actplayer] = -1;
    }
}

void mapact(GLint mapx, GLint mapy)
{
    creature *b;
    bobj *d;
    obj *e;
    bool f = false;
    if (mapx >= 0 && mapx < gmapx && mapy >= 0 && mapy < gmapy)
    {
        //gmap[mapx][mapy].param[0] = 3;
        if (gmap[mapx][mapy].param[1] == -1)
        {
            switch(gmap[mapx][mapy].param[0])
            {
            case 0:
                if (mb[0].second)
                {
                    if (buildtype[actplayer] == -1)
                    {
                        sendactopoint(mapx, mapy);
                    }
                    else
                    {
                        if (actcreature[actplayer].size() == 1 && mapobj[actplayer].count(actcreature[actplayer][0]) != 0)
                        {
                            b = &mapobj[actplayer][actcreature[actplayer][0]];
                            d = &mapobj[actplayer][actcreature[actplayer][0]].btype[buildtype[actplayer]];
                            e = &gameobj[actplayer][d->t];
                            f = true;
                            //g = abs(b->rposx + b->sx / 2.0f - mapx);
                            //h = abs(b->rposy + b->sy / 2.0f - mapy);
                            for (long i = 0; i < resourcenum; i++)
                            {
                                if (presources[actplayer][i].n < d->r[i].n)
                                {
                                    f = false;
                                }
                            }
                            if (buildobj(mapx + ceil(e->sx / 2.0f) - 1, mapy + ceil(e->sy / 2.0f) - 1, 1, 1, false) && f)
                            {
                                mapobj[actplayer][actcreature[actplayer][0]].bmapx = mapx;
                                mapobj[actplayer][actcreature[actplayer][0]].bmapy = mapy;
                                mapobj[actplayer][actcreature[actplayer][0]].act = 3;
                                mapobj[actplayer][actcreature[actplayer].front()].buildt = buildtype[actplayer];
                                buildtype[actplayer] = -1;
                            } // add good mapx and mapy
                        }
                        else
                        {
                            buildtype[actplayer] = -1;
                        }
                    }
                }
                break;
            case 2: // found resources
                if (mb[0].second)
                {
                    for (long i = 0; i < actcreature[actplayer].size(); i++)
                    {
                        mapobj[actplayer][actcreature[actplayer][i]].act = 4;
                        mapobj[actplayer][actcreature[actplayer][i]].rpos = {mapx, mapy};
                        mapobj[actplayer][actcreature[actplayer][i]].restype = gmap[mapx][mapy].rp[0];
                    }
                }
                break;
            default:
                break;
            }
        }
        else
        {
            if (mb[0].second)
            {
                if (gmap[mapx][mapy].param[1] == actplayer)
                {
                    if (actcreature[actplayer].size() == 1 && gmap[mapx][mapy].param[1] == actplayer && mapobj[actplayer][gmap[mapx][mapy].param[2]].buildp < 1.0f)
                    {
                        f = true;
                        for (long i = 0; i < mapobj[actplayer][actcreature[actplayer][0]].btype.size(); i++)
                        {
                            if (mapobj[actplayer][actcreature[actplayer][0]].btype[i].t == mapobj[actplayer][gmap[mapx][mapy].param[2]].t)
                            {
                                f = false;
                                mapobj[actplayer][actcreature[actplayer][0]].act = 2;
                                mapobj[actplayer][actcreature[actplayer][0]].bcreature = gmap[mapx][mapy].param[2];
                                break;
                            }
                        }
                        if (f)
                        {
                            goto playerclickm1;
                        }
                    }
                    else
                    {
                        playerclickm1:;
                        for (long i = 0; i < actcreature[actplayer].size(); i++)
                        {
                            mapobj[actplayer][actcreature[actplayer][i]].active = false;
                        }
                        if (actcreature[actplayer].size() == 1 && actcreature[actplayer][0] == gmap[mapx][mapy].param[2])
                        {
                            actcreature[actplayer].clear();
                        }
                        else
                        {
                            actcreature[actplayer].clear();
                            actcreature[actplayer].push_back(gmap[mapx][mapy].param[2]);
                            for (long i = 0; i < actcreature[actplayer].size(); i++)
                            {
                                mapobj[actplayer][actcreature[actplayer][i]].active = true;
                            }
                        }
                        buildnum[actplayer] = 0;
                        buildtype[actplayer] = -1;
                    }
                }
                else
                {
                    for (long i = 0; i < actcreature[actplayer].size(); i++)
                    {
                        mapobj[actplayer][actcreature[actplayer][i]].act = 1; // attack
                        mapobj[actplayer][actcreature[actplayer][i]].attackobj = gmap[mapx][mapy].param[2];
                        mapobj[actplayer][actcreature[actplayer][i]].attackplayer = gmap[mapx][mapy].param[1];
                    }
                }
            }
        }
        mouseactclear();
        for (long i = 0; i < 3; i++)
        {
            if (mb[i].second)
            {
                mb[i] = {false, false};
            }
        }
        // map
    }
}

void sendactopoint(GLint x, GLint y) // send active creatures to point
{
    GLint midactcx = 0, midactcy = 0; // // middle of act creatures x, middle of act creatures y
    pair<GLfloat, GLfloat> a;
    creature *b;
    // middle point ??
    for (long i = 0; i < actcreature[actplayer].size(); i++)
    {
        midactcx += mapobj[actplayer][actcreature[actplayer][i]].posx;
        midactcy += mapobj[actplayer][actcreature[actplayer][i]].posy;
    }
    midactcx /= max((long)actcreature[actplayer].size(), 1L);
    midactcy /= max((long)actcreature[actplayer].size(), 1L);
    for (long i = 0; i < actcreature[actplayer].size(); i++)
    {
        b = &mapobj[actplayer][actcreature[actplayer][i]];
        b->act = 0; // go
        a = findcpos(*b, x, y);
        mapobj[actplayer][actcreature[actplayer][i]].nx = a.first + mapobj[actplayer][actcreature[actplayer][i]].posx - midactcx;
        mapobj[actplayer][actcreature[actplayer][i]].ny = a.second + mapobj[actplayer][actcreature[actplayer][i]].posy - midactcy;
    }
}

void playclick()
{
    GLfloat menux = 0.0f, menuy = 0.0f, mapxf = 0.0f, mapyf = 0.0f;
    GLint mapx = 0, mapy = 0;
    pair<GLfloat, GLfloat> mbsmenupos; // mouse button (mb[0]) start menu position
    pair<GLfloat, GLfloat> mbsmapposf; // GLfloat
    pair<GLint, GLint> mbsmappos; // mouse button (mb[0]) start map position
    GLint c = 0;
    bool g = false;
    searchmpos(mp[0], mp[1], &menux, &menuy);
    menux -= 0.09f;
    menuy -= 0.073f;
    bsearchmpos(mp[0], mp[1], &mapxf, &mapyf);
    mapx = (GLint)mapxf;
    mapy = (GLint)mapyf;
    mapx += mdrawx;
    mapy += mdrawy;
    gmapposx = mapx;
    gmapposy = mapy;
    for (long i = 0; i < actcreature.size(); i++)
    {
        for (long j = 0; j < actcreature[i].size(); j++)
        {
            if (mapobj[i].count(actcreature[i][j]) == 0)
            {
                if (j != actcreature[i].size() - 1)
                {
                    swap(actcreature[i][j], actcreature[i][actcreature[i].size() - 1]);
                }
                actcreature[i].pop_back();
                j--;
            }
        }
    }
    if (menux >= 0.056f && menuy >= 0.032f)
    {
        if (mb[0].second)
        {
            if (actcreature[actplayer].size() == 0)
            {
                mdrawx = max(min((GLint)(getmenuminimapx(menux) - mviewx / 2), gmapx), 0);
                mdrawy = max(min((GLint)(getmenuminimapy(menuy) - mviewy / 2), gmapy), 0);
            }
            else
            {
                sendactopoint(getmenuminimapx(menux), getmenuminimapy(menuy)); // mapact() ??
            }
        }
        if (mb[1].second)
        {
            mouseactclear();
        }
        for (long i = 0; i < 3; i++)
        {
            mb[i] = {false, false};
        }
        return;
        // minimap menu
    }
    if (inmenu(menux, menuy))
    {
        if (menux >= 0.056f && menux < 0.098f)
        {
            if (actcreature[actplayer].size() == 1)
            {
                if (mb[0].second)
                {
                    if (menuy >= -0.044f && menuy < 0.022f)
                    {
                        c = buildnum[actplayer] + (-(menuy - 0.022f)) / 0.0165f;
                        if (c < mapobj[actplayer][actcreature[actplayer][0]].btype.size())
                        {
                            buildtype[actplayer] = c;
                            bdrawsx[actplayer] = gameobj[actplayer][mapobj[actplayer][actcreature[actplayer][0]].btype[c].t].sx;
                            bdrawsy[actplayer] = gameobj[actplayer][mapobj[actplayer][actcreature[actplayer][0]].btype[c].t].sy;
                        }
                    }
                    if (menuy >= 0.022f && menuy < 0.032f)
                    {
                        buildnum[actplayer]--;
                        if (buildnum[actplayer] < 0)
                        {
                            buildnum[actplayer] = 0;
                        }
                    }
                    if (menuy >= -0.054f && menuy < -0.044f)
                    {
                        if (buildnum[actplayer] + 4 < mapobj[actplayer][actcreature[actplayer][0]].btype.size())
                        {
                            buildnum[actplayer]++;
                        }
                    }
                }
            }
            // build menu
        }
        for (long i = 0; i < 3; i++)
        {
            if (mb[i].second)
            {
                mb[i] = {false, false};
            }
        }
        // menu
    }
    else
    {
        if (outsidemenu(menux, menuy))
        {
            g = false;
            if (menux >= 0.096f)
            {
                g = true;
                if (mgametime >= mapmovetimer + mapmovetime)
                {
                    mdrawx++;
                }
            }
            if (menux < -0.096f)
            {
                g = true;
                if (mgametime >= mapmovetimer + mapmovetime)
                {
                    mdrawx--;
                }
            }
            if (menuy >= 0.07f)
            {
                g = true;
                if (mgametime >= mapmovetimer + mapmovetime)
                {
                    mdrawy++;
                }
            }
            if (menuy < -0.073f)
            {
                g = true;
                if (mgametime >= mapmovetimer + mapmovetime)
                {
                    mdrawy--;
                }
            }
            if (mgametime >= mapmovetimer + mapmovetime)
            {
                mapmovetime = mgametime;
            }
        }
        else
        {
            if (mb[0].first)
            {
                searchmpos(mbpos[0][0].first, mbpos[0][0].second, &(mbsmenupos.first), &(mbsmenupos.second));
                mbsmenupos.first -= 0.09f;
                mbsmenupos.second -= 0.073f;
                bsearchmpos(mbpos[0][0].first, mbpos[0][0].second, &(mbsmapposf.first), &(mbsmapposf.second));
                mbsmappos = mbsmapposf;
                mbsmappos.first += mdrawx;
                mbsmappos.second += mdrawy;
            }
            if (!inmenu(mbsmenupos.first, mbsmenupos.second) && !outsidemenu(mbsmenupos.first, mbsmenupos.second) && mb[0].first)
            {
                glTranslatef(0.0f, 0.0f, -0.18f);
                glColor3f(0.0f, 0.0f, 0.0f);
                squareline(menux, menuy, mbsmenupos.first, mbsmenupos.second, 2.0f);
                glTranslatef(0.0f, 0.0f, 0.18f);
                if (mb[0].second && (mapx != mbsmappos.first || mapy != mbsmappos.second) && distf({mapxf, mapyf}, mbsmapposf) >= 2.0f) // const
                {
                    set<GLint> s;
                    mb[0].first = false;
                    mb[0].second = false;
                    for (long i = 0; i < actcreature[actplayer].size(); i++)
                    {
                        mapobj[actplayer][actcreature[actplayer][i]].active = false;
                    }
                    actcreature[actplayer].clear();
                    for (long i = max(min(min(mapx, mbsmappos.first), gmapx - 1), 0); i < min(max(max(mapx, mbsmappos.first), 0), gmapx - 1); i++)
                    {
                        for (long j = max(min(min(mapy, mbsmappos.second), gmapy - 1), 0); j < min(max(max(mapy, mbsmappos.second), 0), gmapy - 1); j++)
                        {
                            if (gmap[i][j].param[1] == actplayer && s.count(gmap[i][j].param[2]) == 0)
                            {
                                actcreature[actplayer].push_back(gmap[i][j].param[2]);
                                s.insert(gmap[i][j].param[2]);
                            }
                        }
                    }
                    for (long i = 0; i < actcreature[actplayer].size(); i++)
                    {
                        mapobj[actplayer][actcreature[actplayer][i]].active = true;
                    }
                    buildnum[actplayer] = 0;
                    buildtype[actplayer] = -1;
                    return;
                }
            }
            mapact(mapx, mapy);
        }
    }
}

void drawmenu()
{
    glTranslatef(0.0f, 0.0f, -0.18f);
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    square2d(-0.098f, -1.0f, -1.0f, 1.0f);
    square2d(-1.0f, -0.074f, 1.0f, -1.0f);
    square2d(0.098f, -1.0f, 1.0f, 1.0f);
    square2d(-1.0f, 0.074f, 1.0f, 1.0f);
    glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
    square2d(0.056f, -0.074f, 0.098f, 0.074f);
    square2d(-0.098f, -0.074f, 0.056f, -0.06f);
    square2d(-0.098f, 0.074f, 0.056f, 0.064f);
    square2d(-0.098f, -0.06, -0.083f, 0.064f);
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    squareline(0.056f, -0.074f, 0.098f, 0.074f, 2.0f);
    squareline(-0.098f, -0.074f, 0.056f, -0.06f, 2.0f);
    squareline(-0.098f, 0.074f, 0.056f, 0.064f, 2.0f);
    squareline(-0.098f, -0.06f, -0.083f, 0.064f, 2.0f);
    squareline(0.056f, 0.032f, 0.098f, 0.074f, 2.0f);
    squareline(0.056f, 0.022f, 0.098f, 0.032f, 2.0f);
    squareline(0.056f, -0.054f, 0.098f, -0.044f, 2.0f);
    squareline(0.056f, -0.074f, 0.098f, -0.054f, 2.0f);
    for (long i = 0; i < 4; i++)
    {
        squareline(0.056f, 0.022f - i * 0.0165f, 0.098f, 0.022f - (i + 1) * 0.0165f, 2.0f);
    }
    glTranslatef(0.0f, 0.0f, 0.18f);
}

color choosecolor(long i, long j, vector<vector<melem> > &m)
{
    color c;
    switch(m[i][j].param[0])
    {
    case 0: // place
        c.r = 0.3f;
        c.g = 0.5f;
        c.b = 0.0f;
        c.a = 1.0f;
        break;
    case 1: // thing
        c.r = 0.2f;
        c.g = 0.1f;
        c.b = 0.0f;
        c.a = 1.0f;
        break;
    case 2: // resourses
        c.g = 1.0f;
        c.b = 0.0f;
        c.r = 1.0f;
        c.a = 1.0f;
        break;
    default:
        c.r = 0.0f;
        c.g = 0.0f;
        c.b = 0.0f;
        c.a = 1.0f;
        break;
    }
    return c;
}

void drawminimap()
{
    GLfloat a = (GLfloat)gmapx / (GLfloat)gminimapx, b = (GLfloat)gmapy / (GLfloat)gminimapy;
    color c;
    GLfloat k = min(0.04f / gmapx, 0.04f / gmapy);
    glTranslatef(0.057f, 0.033f, -0.18f);
    for(long i = 0; i < gminimapx; i++)
    {
        for (long j = 0; j < gminimapy; j++)
        {
            c = choosecolor(i, j, gminimap);
            // choose color on sqrt
            c.setc();
            square2d(i * a * k, j * b * k,  (i + 1) * a * k, (j + 1) * b * k);
        }
    }
    for (long i = 0; i < mapobj.size(); i++)
    {
        for (auto it = mapobj[i].begin(); it != mapobj[i].end(); it++)
        {
            glColor3f(pcolor[i].R, pcolor[i].G, pcolor[i].B);
            square2d(it->second.posx * k, it->second.posy * k, (it->second.posx + it->second.sx) * k, (it->second.posy + it->second.sy) * k);
        } // +1 or +2 * size ??
    }
    glColor3f(0.0f, 0.0f, 0.0f);
    squareline(mdrawx * k, mdrawy * k, (mdrawx + mviewx) * k, (mdrawy + mviewy) * k, 1.0f);
    glTranslatef(-0.057f, -0.033f, 0.18f);
}

void drawmap()
{
    color c;
    for (GLint i = 0; i < mviewx; i++)
    {
        for (GLint j = 0; j < mviewy; j++)
        {
            if ((i + mdrawx) < gmapx && (i + mdrawx) >= 0 && (j + mdrawy) < gmapy && (j + mdrawy) >= 0)
            {
                c = choosecolor(i + mdrawx, j + mdrawy, gmap);
                c.setc();
                glTranslatef(-16.6f, -12.0f, -mdeep);
                square2d(i * msqx, j * msqy, (i + 1) * msqx, (j + 1) * msqy);
                glTranslatef(16.6f, 12.0f, mdeep);
            }
        }
    }

    glTranslatef(-16.6f, -12.0f, -mdeep);
    for (long i = 0; i < playernum; i++) // playernum
    {
        for (long j = 0; j < mapobj[i].size(); j++)
        {
            mapobj[i][j].draw();
        }
    }
    for (long i = 0; i < playernum; i++)
    {
        for (long j = 0; j < attackthing[i].size(); j++)
        {
            attackthing[i][j].draw();
        }
    }
    glTranslatef(16.6f, 12.0f, mdeep);
    for (GLint i = 0; i < mviewx * 2; i++)
    {
        for (GLint j = 0; j < mviewy * 2; j++)
        {
            if ((i + mdrawx) < gmapx && (i + mdrawx) >= 0 && (j + mdrawy) < gmapy && (j + mdrawy) >= 0)
            {
                if (buildtype[actplayer] != -1 && actcreature[actplayer].size() == 1 && mapobj[actplayer].count(actcreature[actplayer][0]) != 0 && gmapposx + bdrawsx[actplayer] / 2 >= i + mdrawx && gmapposx - bdrawsx[actplayer] + bdrawsx[actplayer] / 2 < i + mdrawx && gmapposy + bdrawsy[actplayer] / 2 >= j + mdrawy && gmapposy - bdrawsy[actplayer] + bdrawsy[actplayer] / 2 < j + mdrawy)
                {
                    if (gmap[i + mdrawx][j + mdrawy].param[0] == 0 && gmap[i + mdrawx][j + mdrawy].param[1] == -1)
                    {
                        c.r = 1.0f;
                        c.g = 1.0f;
                        c.b = 1.0f;
                    }
                    else
                    {
                        c.r = 0.0f;
                        c.g = 0.0f;
                        c.b = 0.0f;
                    }
                    c.setc();
                    glTranslatef(-16.6f, -12.0f, -mdeep);
                    square2d(i * msqx, j * msqy, (i + 1) * msqx, (j + 1) * msqy);
                    glTranslatef(16.6f, 12.0f, mdeep);
                }
            }
        }
    }
}

void attackstep()
{
    for (long i = 0; i < playernum; i++)
    {
        for (long j = 0; j < attackthing[i].size(); j++)
        {
            if (attackthing[i][j].go())
            {
                if (j != attackthing.size() - 1)
                {
                    swap(attackthing[i][j], attackthing[i][attackthing[i].size() - 1]);
                }
                attackthing[i].pop_back();
                j--;
            }
        }
    }
}

void checkdrawstep() // ??
{
    mdrawx = max(mdrawx, -1);
    mdrawx = min(mdrawx, gmapx - mviewx + 1);
    mdrawy = max(mdrawy, -1);
    mdrawy = min(mdrawy, gmapy - mviewy + 1);
}

bool lose()
{
    if (mapobj[actplayer].size() == 0)
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, -0.01f);
        square2d(-1.0f, -1.0f, 1.0f, 1.0f);
        glTranslatef(0.0f, 0.0f, 0.01f);
        if (mb[0].first || mb[1].first || mb[2].first)
        {
            exit(0);
        }
        return true;
    }
    return false;
}

bool win()
{
    for (long i = 0; i < mapobj.size(); i++)
    {
        if (i != actplayer)
        {
            if (mapobj[i].size() != 0)
            {
                return false;
            }
        }
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, -0.01f);
    square2d(-1.0f, -1.0f, 1.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, 0.01f);
    if (mb[0].first || mb[1].first || mb[2].first)
    {
        exit(0);
    }
    return true;
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();
	mgametime = glutGet(GLUT_ELAPSED_TIME);
	settime();
	if (!lose() && !win())
    {
        checkdrawstep();
        drawmap();
        creaturestep();
        attackstep();
        playclick();
        checkdrawstep();
        drawmenu();
        drawminimap();
    }
    breaktime();
    glutSwapBuffers();
    glFlush();
}

void keyboard(unsigned char button, int kx, int ky)
{
    switch (button)
    {
        case 27:
            exit(0);
            break;
        case '1':
            actplayer = 1 - actplayer;
            break;
        default:
            break;
    }

}

void keyboards(int button, int kxs, int kys) //special keyboard
{
	switch (button)
	{
		case GLUT_KEY_F1:
			exit(0);
			break;
        case GLUT_KEY_UP:
            mdrawy++;
            break;
        case GLUT_KEY_DOWN:
            mdrawy--;
            break;
        case GLUT_KEY_LEFT:
            mdrawx--;
            break;
        case GLUT_KEY_RIGHT:
            mdrawx++;
            break;
		default:
			break;
	}
}

void mouse(int button, int click, int mx, int my)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			if (click == GLUT_DOWN)
			{
			    mbpos[0][0] = {mx, my};
                //searchmpos(mx, my, &mbpos[0][0].first, &mbpos[0][0].second);
			    mb[0].first = true;
			}
			else
			{
			    mbpos[1][0] = {mx, my};
			    //searchmpos(mx, my, &mbpos[1][0].first, &mbpos[1][0].second);
				mb[0].second = true;
			}
			break;
		case GLUT_MIDDLE_BUTTON:
			if (click == GLUT_DOWN)
			{
			    mbpos[0][2] = {mx, my};
			    //searchmpos(mx, my, &mbpos[0][2].first, &mbpos[0][2].second);
			    mb[2].first = true;
			}
			else
			{
			    mbpos[1][2] = {mx, my};
			    //searchmpos(mx, my, &mbpos[1][2].first, &mbpos[1][2].second);
				mb[2].second = true;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if (click == GLUT_DOWN)
			{
			    mbpos[0][1] = {mx, my};
			    //searchmpos(mx, my, &mbpos[0][1].first, &mbpos[0][1].second);
			    mb[1].first = true;
			}
			else
			{
			    mbpos[1][1] = {mx, my};
			    //searchmpos(mx, my, &mbpos[1][1].first, &mbpos[1][1].second);
				mb[1].second = true;
			}
			break;
		default:
			break;
	}
	mp[0] = mx;
	mp[1] = my;
}

void pmotion(GLint mx, GLint my) // passive motion
{
	mp[0] = mx;
	mp[1] = my;
	//mb[0] = false;
	//mb[1] = false;
	//mb[2] = false;
}

void amotion(GLint mx, GLint my) // active motion
{
	mp[0] = mx;
	mp[1] = my;
	//mb[0] = false;
	//mb[1] = false;
	//mb[2] = false;
}

void window(GLint w, GLint h)
{
	if (h == 0)
	{
		h = 1;
	}
	wp[0] = w;
	wp[1] = h;
	GLfloat view = w * 1.0f / h;
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glViewport (0, 0, w, h);
	//glFrustum (-0.1f, 0.1f, -0.074f, 0.074f, 0.09f, 100.f);
	gluPerspective (45, view, 0.01f, 100.0f);
	glMatrixMode (GL_MODELVIEW);
	//glEnable (GL_DEPTH_TEST);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("RTS");
	initGL();
	initmap();
	init();
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboards);
	glutMouseFunc(mouse);
	glutMotionFunc(amotion);
    glutPassiveMotionFunc(pmotion);
	glutReshapeFunc(window);
	glutIdleFunc(display);
	glutDisplayFunc(display);
	glutMainLoop ();
}

