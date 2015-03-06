// 'number_of_ones' is how many '1' elements you have in your hands. Which should I put they in order to get the longest sequence?
// 'W' 'L' are the corresponding right and left index (boundaries)
A = [0,1,0,0,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0]
number_of_ones = 2;

wL = 0; wR = 0; 
nZero = 0;
bestWindowWidth = -1;

while(wR < A.length){	
	if (nZero <= number_of_ones){
		if (A[wR] === 0) nZero++; 
		wR++;
	};

	if (nZero > number_of_ones){ 
		if (A[wL] === 0) nZero--;
		wL++;
	};

//	console.log("nzero: "+ nZero  + " WR: "+wR+" + WL: " +wL );

	if (wR - wL > bestWindowWidth){
		bestWindowWidth = wR - wL;
		bestWR = wR;
		bestWL = wL;
	};	
};

console.log("R ", bestWR);
console.log("L ", bestWL);
console.log("Best ", bestWindowWidth);
