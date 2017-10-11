#include "raycast.h"
/*pixheight = float(h) / float(M); // the height of one pixel
pixwidth = float(w) / float(N); // the width of one pixel
for(i = 0; i < M; i++){ // for each row
	py = cy - h / 2 + pixheight * (i + 0.5); // y coord of row
	for(j = 0; j < N; j++){ // for each column
		px = cx - w / 2 + pixwidth * (j + 0.5); // x coord of column
		pz = −zp; // z coord is on screen
		ur = p/kpk; // unit ray vector
		x = shoot(ur); // return position of first hit
		image[i][j] = shade(x); // pixel colored by object hit
	}
}*/