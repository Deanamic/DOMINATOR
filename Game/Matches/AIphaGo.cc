#include "Player.hh"
/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME AIphaGo


struct PLAYER_NAME : public Player {

    static Player * factory() {
        return new PLAYER_NAME;
    }

    /* TYPEDEF */
        using VI = vector<int>;
        using VVI = vector<VI>;

        using VD = vector<double>;
        using VVD = vector <VD>;

        using VB = vector<bool>;
        using VVB = vector<VB>;

    /* VECTORS */
        VI W, K, F;
        //VVB OCC;
        int land;
        VVB BFS;
        VVB HAUNT;
        queue<Pos> Clean;
        queue

        /* CONST */
            int Column, Row;
            const double INF = 1e6;
            const double EPS = 1e-6;

            /* TEAM */
                const int T_MINE = 0;
                const int T_ENEMY = 1;
                const int T_ALL = 2;


            /* WITCH */
                /* KILL FARMER */
                    const double WK_FARMER = 30.0;
                    const double WK_FARMER_C = 1.5;
                    const int WK_FARMER_D = 13;

                /* KILL KNIGHT */
                    const double WK_KNIGHT = 20.0;
                    const double WK_KNIGHT_C = 1.5;
                    const int WK_KNIGHT_D = 15;

                /* WITCH OVERLAP */
                    const double WO_WITCH = -50.0;
                    const double WO_WITCH_C = 2.0;
                    const int WO_WITCH_D = 10;

                /* PROTECT LAND better without?
                     *  const double WP_LAND = -5.0;
                     *  const double WP_LAND_C = 1.0;
                     *  const int WP_LAND_D = -1;
                     */

            /* FARMER */
                /* RUN FROM WITCH */
                    const double FR_WITCH = -INF;
                    const double FR_WITCH_C = 1.1;
                    const int FR_WITCH_D = 4;

                /* FARM LAND */
                    const double FF_LAND = 2000.0;
                    const double FF_LAND_C = 4;
                    const int FF_LAND_D = 25;

                /* FARM ADJLAND */
                    const double FF_ADJLAND = 0;
                    const double FF_ADJLAND_C = 50.0;
                    const int FF_ADJLAND_D = 2;

                /* RUN FROM KNIGHT */
                    const double FR_KNIGHT = -8000;
                    const double FR_KNIGHT_C = 4.0;
                    const int FR_KNIGHT_D = 0;

            /* KNIGHT */
                /* KILL FARMER */
                    const double KK_FARMER = 30.0;
                    const double KK_FARMER_C = 1.5;
                    const int KK_FARMER_D = 17;

                /* KILL KNIGHT */
                    const double KK_KNIGHT = 60.0;
                    const double KK_KNIGHT_C = 2.5;
                    const int KK_KNIGHT_D = 0;

                /* RUN FROM WITCH */
                    const double KR_WITCH = -INF;
                    const double KR_WITCH_C = 1.1;
                    const int KR_WITCH_D = 4;

                    const double KK_TAGGED = 100.0;

    /* STRUCTS */
        struct S_BFS {
            Pos P;
            int d;
            double value;
        };


    /* AUX FUNCTION */
        bool FB_Range(const Pos& P,const int& i,const int& j){
            return P.i + i > -1 and P.i + i < Row and P.j + j > -1 and P.j + j < Column;
        }

        bool FB_Dist(const int& i,const int& j,const int& r){
            return abs(i) + abs(j) < r;
        }

        bool FB_OBS(const string& s,const Unit& U){
            for (int i = 0; i < 3; ++i) {
                if (s[i] == '1' and U.type == i) return false;
            }
            return true;
        }
        Dir FD_Dir(const int& i,const int& j){
            if (i > 0) {
                if (j < 0) return LB;
                if (j == 0) return Bottom;
                if (j > 0) return BR;
            }
            if (i == 0) {
                if (j > 0) return Right;
                if (j == 0) return None;
                if (j < 0) return Left;
            }
            if (i < 0) {
                if (j > 0) return RT;
                if (j == 0) return Top;
                if (j < 0) return TL;
            }
            return None;
        }

    /* PROPAGATION FUNCTION */
        void FV_PROPAGATION(VVD& PRIORITY,const double& Value,const double& Change,const int& Distance,const Pos& P,const string& obstacle,const int& team,const bool& div,const bool& diagonal,const bool& haunted){
            queue<S_BFS> Q;
            Q.push({ P, 0, Value });
            while (!Q.empty()) {
                S_BFS H = Q.front(); Q.pop();
                Cell c = cell(H.P);
                if (H.d <= Distance and FB_Range(H.P, 0, 0) and !BFS[H.P.i][H.P.j] and c.type != Wall) {
                    if (c.id == -1 or FB_OBS(obstacle, unit(c.id))) {
                        if (team == T_ALL or H.P == P or(team == T_MINE and c.owner == 0) or(team == T_ENEMY and c.owner != 0)) {
                            if (!haunted or !HAUNT[H.P.i][H.P.j]) {
                                PRIORITY[H.P.i][H.P.j] += H.value;
                                BFS[H.P.i][H.P.j] = true;
                                if (diagonal) {
                                    for (int i = 0; i < 8; ++i) {
                                        Q.push({ H.P + Dir(i), H.d + 1, (div ? H.value / Change : H.value + Change) });
                                    }
                                } else {
                                    for (int i = 0; i < 4; ++i) {
                                        Q.push({ H.P + Dir(2 * i), H.d + 1, (div ? H.value / Change : H.value + Change) });
                                    }
                                }
                            }
                        }
                    }
                }
            }
            for (int i = -Distance; i < Distance + 1; ++i) {
                for (int j = -Distance; j < Distance + 1; ++j) {
                    if (FB_Range(P, i, j)) {
                        BFS[P.i + i][P.j + j] = false;
                    }
                }
            }
        } // FV_PROPAGATION

    /* WITCH */

        void FV_WITCH_UPDATE(VVD& WP, VVD& KP, VVD& FP, const int& I, const int& J){
            FV_PROPAGATION(FP, FR_WITCH, FR_WITCH_C, FR_WITCH_D, { I, J }, "000", T_ALL, true, false, false);
            FV_PROPAGATION(KP, KR_WITCH, KR_WITCH_C, KR_WITCH_D, { I, J }, "000", T_ALL, true, false, false);
            FV_PROPAGATION(WP, WO_WITCH, WO_WITCH_C, WO_WITCH_D, { I, J }, "000", T_ALL, true, false, false);
        }

        void FV_WITCH_MARK(VVB& HAUNTED,const int& id){
            Unit W = unit(id);
            Pos P = W.pos;

            for (int i = -3; i < 4; ++i) {
                for (int j = -3; j < 4; ++j) {
                    if (FB_Range(P, i, j) and FB_Dist(i, j, 4)) {
                        HAUNT[P.i + i][P.j + j] = true;
                        Clean.push({ P.i + i, P.j + j });
                    }
                }
            }
        }

        void FV_MOVE_W(const int& id, VVD& WP, VVD& KP, VVD& FP){
            Unit W = unit(id);
            Pos P = W.pos;
            Dir d = None;
            double MX = WP[P.i][P.j];

            for (int i = 1; i > -2; --i) {
                for (int j = 1; j > -2; --j) {
                    if (FB_Range(P, i, j) and FB_Dist(i, j, 2)) {
                        Dir d1 = FD_Dir(i, j);
                        Cell c = cell(P + d1);
                        if (c.type != Wall and(c.id == -1 or unit(c.id).type != Witch or c.id == id)) {
                            if (WP[P.i + i][P.j + j] > MX + EPS) {
                                d = d1;
                                MX = WP[P.i + i][P.j + j];
                            }
                        }
                    }
                }
            }
            command(id, d);
            P += d;
            FV_WITCH_UPDATE(WP,KP,FP,P.i,P.j);
        } // FV_MOVE_W


    /* KNIGHT */

        void FV_KNIGHT_UPDATE(VVD& WP, VVD& KP, VVD& FP, const int& I, const int& J, const Cell& c){
            Unit u = unit(c.id);

            if (u.player == 0) {

            } else {
                FV_PROPAGATION(WP, WK_KNIGHT, WK_KNIGHT_C, WK_KNIGHT_D, { I, J }, "001", T_ALL, true, false, false);
                FV_PROPAGATION(FP, FR_KNIGHT, FR_KNIGHT_C, FR_KNIGHT_D, { I, J }, "001", T_ALL, true, true, true);
                FV_PROPAGATION(KP, KK_KNIGHT, KK_KNIGHT_C, KK_KNIGHT_D, { I, J }, "001", T_ALL, true, true, true);
            }
        }

        void FV_MOVE_K(int id, VVD& WP, VVD& KP, VVD& FP){
            Unit K = unit(id);
            Pos P = K.pos;
            Dir d = None;
            double MX = KP[P.i][P.j];

            for (int i = 1; i > -2; --i) {
                for (int j = 1; j > -2; --j) {
                    if (FB_Range(P, i, j)) {
                        Dir d1 = FD_Dir(i, j);
                        Cell c = cell(P + d1);
                        if (c.type != Wall and (c.id == -1 or unit(c.id).type != Witch)) {
                            if (KP[P.i + i][P.j + j] > MX + EPS) {
                                d = d1;
                                MX = KP[P.i + i][P.j + j];
                            }
                        }
                    }
                }
            }
            command(id, d);
            P = P + d;
            Cell c = cell(P);
            //if(c.id != -1 and unit(c.id).type != Witch) KP[P.i][P.j] += KK_TAGGED;
        }

    /* FARMER */
        void FV_FARMER_UPDATE(VVD& WP, VVD& KP, VVD& FP, const int& I, const int& J, const Cell& c){
            Unit u = unit(c.id);

            if (u.player == 0) {
            } else {
                FV_PROPAGATION(WP, WK_FARMER, WK_FARMER_C, WK_FARMER_D, { I, J }, "001", T_ALL, true, false, false);
                FV_PROPAGATION(KP, KK_FARMER, KK_FARMER_C, KK_FARMER_D, { I, J }, "001", T_ALL, true, true, true);
            }
        }

        void FV_MOVE_F(const int& id, VVD& WP, VVD& KP, VVD& FP){
            Unit F = unit(id);
            Pos P = F.pos;
            Dir d = None;
            double MX = FP[P.i][P.j];

            for (int i = 1; i > -2; --i) {
                for (int j = 1; j > -2; --j) {
                    if (FB_Range(P, i, j) and FB_Dist(i, j, 2)) {
                        Dir d1 = FD_Dir(i, j);
                        Cell c = cell(P + d1);
                        if (c.type != Wall and (c.id == -1 or c.id == id) ) {
                            if (FP[P.i + i][P.j + j] > MX + EPS) {
                                d = d1;
                                MX = FP[P.i + i][P.j + j];
                            }
                        }
                    }
                }
            }
            command(id, d);
            P = P + d;
            // FV_FARMER_UPDATE(WP,KP,FP,aux.i,aux.j,cell(P));
        } // FV_MOVE_F

    /* GAME */

    void FV_Update_State(VVD& WP, VVD& KP, VVD& FP){
        for (int I = 0; I < Row; ++I) {
            for (int J = 0; J < Column; ++J) {
                Cell c = cell(I, J);
                if (c.type != Wall) {
                    if ( c.id == -1) {
                        FV_LAND_UPDATE(WP, KP, FP, I, J, c);
                    } else {
                        Unit u = unit(c.id);
                        switch (u.type){
                            case Farmer:
                                FV_FARMER_UPDATE(WP, KP, FP, I, J, c);
                                break;
                            case Knight:
                                FV_KNIGHT_UPDATE(WP, KP, FP, I, J, c);
                                break;
                            case Witch:
                                FV_WITCH_UPDATE(WP, KP, FP, I, J);
                                break;
                        }
                    }
                }
            }
        }
    }

        void FV_LAND_UPDATE(VVD& WP, VVD& KP, VVD& FP, const int& I, const int& J, const Cell& c){
            if (c.owner == 0) {
                // FV_PROPAGATION(WP, WP_LAND, WP_LAND_C, WP_LAND_D, {I,J}, "111", T_ENEMY, false, false, false);
            } else {
                FV_PROPAGATION(FP, FF_ADJLAND, FF_ADJLAND_C, FF_ADJLAND_D, { I, J }, "001", T_ENEMY, false, false, true);
                FV_PROPAGATION(FP, FF_LAND, FF_LAND_C, FF_LAND_D, { I, J }, "111", T_MINE, true, false, true);
            }

        }

        void FV_GET_LAND(){
            for (int I = 0; I < Row; ++I) {
                for (int J = 0; J < Column; ++J) {
                    Cell c = cell(I, J);
                    if (c.type != Wall) ++land;
                }
            }
        }

    virtual void play() {
        if (round() == 0) {
            Column = cols();
            Row = rows();
            HAUNT = BFS = VVB(Row, VB(Column, false));
            W = witches(0);
            VI W1 =  witches(1), W2 = witches(2), W3 = witches(3);
            land = 0;
            FV_GET_LAND();
        }
        VVD WITCH_PRIORITY, KNIGHT_PRIORITY, FARMER_PRIORITY;
        WITCH_PRIORITY = KNIGHT_PRIORITY = FARMER_PRIORITY = VVD(Row, VD(Column, 0));
        F = farmers(0);
        K = knights(0);


        for (unsigned int i = 0; i < W.size(); ++i) {
            FV_WITCH_MARK(HAUNT, W[i]);
            FV_WITCH_MARK(HAUNT, W2[i]);
            FV_WITCH_MARK(HAUNT, W1[i]);
            FV_WITCH_MARK(HAUNT, W3[i]);
        }


        /* TERREMOTO */
            bool win = true;
            for (int i = 1; i < 4; ++i) {
                if ( (nb_rounds() - round() + 1) * land  + total_score(i) > total_score(0)) win = false;
            }
            if (win and round() != 0) {
                for (unsigned int i = 0; i < F.size(); ++i) {
                    command(F[i], Dir(round() % 2 * 4 + 2));
                }
                for (unsigned int i = 0; i < W.size(); ++i) {
                    command(W[i], Dir(round() % 2 * 4 + 2));
                }
                for (unsigned int i = 0; i < K.size(); ++i) {
                    command(K[i], Dir(round() % 2 * 4 + 2));
                }
            } else {

        FV_Update_State(WITCH_PRIORITY, KNIGHT_PRIORITY, FARMER_PRIORITY);
        for (unsigned int i = 0; i < W.size(); ++i) {
            FV_MOVE_W(W[i], WITCH_PRIORITY, KNIGHT_PRIORITY, FARMER_PRIORITY);
        }
        for (unsigned int i = 0; i < F.size(); ++i) {
            FV_MOVE_F(F[i], WITCH_PRIORITY, KNIGHT_PRIORITY, FARMER_PRIORITY);
        }
        for(unsigned int i = 0; i < K.size(); ++i){
            FV_MOVE_K(K[i],WITCH_PRIORITY,KNIGHT_PRIORITY,FARMER_PRIORITY);
        }
        while (!Clean.empty()) {
            Pos p = Clean.front(); Clean.pop(); HAUNT[p.i][p.j] = false;
        }
    } // play
};

    /* UNUSED CODE */




/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
