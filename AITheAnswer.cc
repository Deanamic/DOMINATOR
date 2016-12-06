#include "Player.hh"

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME TheAnswer

struct PLAYER_NAME : public Player {

    /**
     * Factory: returns a new instance of this class.
     * Do not modify this function.
     */
    static Player* factory() {
        return new PLAYER_NAME;
    }

    /**
     * Types and attributes for your player can be defined here.
     */

    typedef vector<int> VE;
    typedef vector<VE> ME;
    typedef vector<bool> VB;
    typedef vector<VB> MB;
    typedef pair<Pos, Dir> PD;
    typedef pair<int, int> PI;
    typedef vector<double> VD;
    typedef vector<VD> MD;

    int ROWS, COLS;
    double alfa, cpu, cpumax, distmax;
    MB taken, vist;
    VE dirs4, dirs8;

    bool is_taken(int i, int j) {
		return taken[i][j];
    }

    bool is_taken(Pos p) {
        return is_taken(p.i, p.j);
    }
        
    void set_taken(int i, int j) {
        taken[i][j] = true;
    }

    void set_taken(Pos p) {
        set_taken(p.i, p.j);
    }
    
    bool is_vist(int i, int j) {
		return vist[i][j];
    }

    bool is_vist(Pos p) {
        return is_vist(p.i, p.j);
    }
    
    void set_vist(int i, int j) {
		vist[i][j] = true;
    }

    void set_vist(Pos p) {
		set_vist(p.i, p.j);
    }
        
    double farmer_pbfs(Pos p) {
        vist = MB(ROWS, VB(COLS, false));
        queue<Pos> q;
        q.push(p);
        set_vist(p);
        double res = 0;
        double dist = 1;
        while (!q.empty() and dist <= distmax) {
            int aux = q.size();
            double den = pow(dist, alfa);
            while (aux--) {
                p = q.front();
				q.pop();
				if (cell(p).id != -1) {
					Unit u = unit(cell(p).id);
					if (u.type != Farmer and u.player != 0) res -= 1.0/ den;
				}
				if (cell(p).owner != 0) res += 5.0 / den;
				if (cell(p).haunted) res -= 10.0 / den;
				for (int i : dirs4) {
					Pos pf = p + Dir(i);
					if (!is_vist(pf)) {
						if (cell(pf).type == Wall) {
							res -= .1 / den;
						} else {
							set_vist(pf);
							q.push(pf);
						}
					}
				}		
            }
			dist++;
        }
        return res;
    }
    
    double witch_pbfs(Pos p, int idw) {
        vist = MB(ROWS, VB(COLS, false));
        queue<Pos> q;
        q.push(p);
        set_vist(p);
        double res = 0;
        double dist = 1;
        while (!q.empty() and dist <= distmax) {
            int aux = q.size();
            double den = pow(dist, alfa);
            while (aux--) {
                p = q.front();
				q.pop();
				if (cell(p).id != -1) {
					Unit u = unit(cell(p).id);
					if (u.player != 0 and u.type != Witch) res += 10.0 / den;
					else if (u.id != idw)res -= 1.0 / den;
				}
				for (int i : dirs4) {
					Pos pf = p + Dir(i);
					if (!is_vist(pf)) {
						if (cell(pf).type == Wall) {
							res -= .1 / den;
						} else {
							set_vist(pf);
							q.push(pf);
						}
					}
				}		
            }
			dist++;
        }
        return res;
    }
    
    double knight_pbfs(Pos p) {
        vist = MB(ROWS, VB(COLS, false));
        queue<Pos> q;
        q.push(p);
        set_vist(p);
        double res = 0;
        double dist = 1;
        while (!q.empty() and dist <= distmax) {
            int aux = q.size();
            double den = pow(dist, alfa);
            while (aux--) {
                p = q.front();
				q.pop();
				if (cell(p).id != -1) {
					Unit u = unit(cell(p).id);
					if (u.player != 0 and u.type != Witch) res += 100.0 / den;
					else res -= 1.0 / den;
				}
				if (cell(p).haunted) res -= 10.0 / den;
				for (int i : dirs4) {
					Pos pf = p + Dir(i);
					if (!is_vist(pf)) {
						if (cell(pf).type == Wall) {
							res -= .1 / den;
						} else {
							set_vist(pf);
							q.push(pf);
						}
					}
				}		
            }
			dist++;
        }
        return res;
    }

	void move_farmer(int id) {
		Dir d = None;
		Pos p = unit(id).pos;
		double best = farmer_pbfs(p);
		for (int i : dirs4) {
			if (cell(p + Dir(i)).type != Wall and !is_taken(p + Dir(i))) {
				double aux = farmer_pbfs(p + Dir(i));
				if (aux > best) {
					best = aux;
					d = Dir(i);
				}
			}
		}
        command(id, d);
        set_taken(p + d);
    }

    void move_witch(int id) {
		Dir d = None;
		Pos p = unit(id).pos;
		double best = witch_pbfs(p, id);
		for (int i : dirs4) {
			if (cell(p + Dir(i)).type != Wall and !is_taken(p + Dir(i)) and cell(p + Dir(i)).id == -1) {
				double aux = witch_pbfs(p + Dir(i), id);
				if (aux > best) {
					best = aux;
					d = Dir(i);
				}
			}
		}
        command(id, d);
        set_taken(unit(id).pos + d);
    }

    void move_knight(int id) {
        Pos p = unit(id).pos;
		Dir d = None;
		//el 10 es patilla
		double salut = 0;
		if (unit(id).health < 100) salut = 10;
		double best = knight_pbfs(p) + salut;
		for (int i : dirs8) {
			if (cell(p + Dir(i)).type != Wall and !is_taken(p + Dir(i))) {
				double aux = knight_pbfs(p + Dir(i));
				if (aux > best) {
					best = aux;
					d = Dir(i);
				}
			}
		}
        command(id, d);
        set_taken(p + d);
    }

    /**
     * Play method, invoked once per each round.
     */
    void play() {
		cpu = status(0);
		cpumax = 0.95;
		alfa = 2;
		if (cpu > cpumax) distmax = 1;
		else distmax = 200;
        ROWS = rows();
        COLS = cols();
        taken = MB(ROWS, VB(COLS, false));
        dirs8 = random_permutation(8);
        dirs4 = random_permutation(4);
		for (int& i : dirs4) i *= 2;
		
        VE w = witches(0);
        for (int id : w) move_witch(id);
        VE k = knights(0);
        for (int id : k) move_knight(id);
        VE f = farmers(0);
        for (int id : f) move_farmer(id);
    }

};
/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
