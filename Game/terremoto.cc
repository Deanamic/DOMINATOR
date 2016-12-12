int land;

void FV_GET_LAND(){
    for (int I = 0; I < Row; ++I) {
        for (int J = 0; J < Column; ++J) {
            Cell c = cell(I, J);
            if (c.type != Wall) ++land;
        }
    }
}
  
if (round() == 0) {
   land = 0;
   FV_GET_LAND();
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
  }
