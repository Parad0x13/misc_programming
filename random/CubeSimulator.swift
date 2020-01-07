// [TODO] Add legalese here

/*  Another try at explaining the development of a system... ignore bottom stuff
    n^3 simulator. Designed to be fast
    All types of moves will be pre-calculated and simply applied when needed
    Basic concept of operation:
    We can visualize the cube faces as a 2D array of cubies from top-left to bottom-right:
        00  01
        02  03
    And we can organize these faces (Up, Down, Left, Right, Front, Back) in a larger 2D array:
        D B D
        L U R
        D F D
        * Note: We replicate D on each corner for rotational symmetry later on
    The full blown up Cubie-Mapping array for a 2x2x2 will look like:
        05 04 23 22 05 04
        07 06 21 20 07 06
        10 08 00 01 13 15
        11 09 02 03 12 14
        05 04 16 17 05 04
        07 06 18 19 07 06
        * Note: All cubies will be visualized in this array as if the cube is
                flattened and you are looking at the UP face from the front
                There is a good reason for this later on
    For translations we will use an indexing array:
        00 01 02 03 04 05
        06 07 08 09 10 11
        12 13 14 15 16 17
        18 19 20 21 22 23
        24 25 26 27 28 29
        30 31 32 33 34 35
    A Z-Axis rotation can be coded as such:
        D B D     L B'L
        L U R --> U R D
        D F D     L F'L
        * Note: Some faces need to be rotated (indicated by the prime notation)
                The B face is rotated 90 degrees and the F face is rotated -90 degrees
        * Note: Axis Translation tables will be generated dynamically once during initialization
    An example of a Z-Axis rotation:
        00 01 02 03 04 05       12 13 08 02 12 13
        06 07 08 09 10 11       18 19 09 03 18 19
        12 13 14 15 16 17  -->  14 15 16 17 00 01
        18 19 20 21 22 23       20 21 22 23 06 07
        24 25 26 27 28 29       12 13 27 33 12 13
        30 31 32 33 34 35       18 19 26 32 18 19
        * [TODO] Ensure the hidden faces are properly rotated/placed
    An X-Axis rotation can be coded as such:
        D B D     B U B
        L U R --> L'F R'
        D F D     B D B
    A Y-Axis rotation can be done by simply rotating the translation array 90 degrees:
        30 24 18 12 06 00
        31 25 19 13 07 01
        32 36 20 14 08 02
        33 27 21 15 09 03
        34 28 22 16 10 04
        35 29 23 17 11 05
    Layer rotations can be encoded by rotating only the inner affected cubies 90 degrees
    An example of a 2x2x2's layer 1 rotation:
        XX XX XX XX XX XX     00 01 02 03 04 05     00 01 02 03 04 05
        XX XX 19 13 XX XX     06 07 XX XX 10 11     06 07 19 13 10 11
        XX 36 20 14 08 XX  +  12 XX XX XX XX 17  =  12 36 20 14 08 17
        XX 27 21 15 09 XX     18 XX XX XX XX 23     18 27 21 15 09 23
        XX XX 22 16 XX XX     24 25 XX XX 28 29     24 25 22 16 28 29
        XX XX XX XX XX XX     30 31 32 33 34 35     30 31 32 33 34 35
        * Note: This process will be done automatically once during initialization
    Translations for every desired movement will have it's own translation table:
        Layer movements for each slice
        The layer movement that includes all layers is the axis rotation with respect to that face
            X-Axis is with respect to the R face
            Y-Axis is with respect to the U face
            Z-Axis is with respect to the F face
        * Note: Slice movements are not needed as they can be replicated with layer + axis rotations
    All translation tables can be stored as such:
        translation_tables[U, D, L, R, F, B][Layer 1, Layer 2, ..., Layer N (Axis Rotation)]
    We can turn the Cubie-Mapping 2D array into a 1D array:
        05 04 23 22 05 04
        07 06 21 20 07 06
        10 08 00 01 13 15
        11 09 02 03 12 14
        05 04 16 17 05 04
        07 06 18 19 07 06
        Becomes: 05 04 23 22 05 04 07 06 21 20 07 06 10 08 00 01 13 15 11 09 02 03 12 14 05 04 16 17 05 14 07 06 18 19 07 06
    We can also turn the translation 2D arrays into 1D arrays:
        00 01 02 03 04 05
        06 07 08 09 10 11
        12 13 14 15 16 17
        18 19 20 21 22 23
        24 25 26 27 28 29
        30 31 32 33 34 35
        Becomes: 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35
    Even though both arrays are 1D the application of translations still work as intended
    It's possible to serialize all translation tables this way and store it as a single blob
    It is nessisary to consider what data-type you need since you'll need it to store at least
        the max value of the largest index cubie. The equation for this is 6n^2
        [TODO] Add the equation for the final blob size here
    Once you have this blob you only need to bitshift the blob to get to the translation table you want
    Translation_Serialized will be formatted as such:
        [U Layer 1] ... [U Layer n] [D Layer 1] ... ... [B Layer n]
*/

/*
    The cube will be represented as a squashed 2D array where the UP face is
    in the very center and the unseen DOWN face is in any of the empty 4 corners.
    All calculations and transformations will be done dynamically. The cubie
    values will be calculated from top-left to bottom-right in a linear fashion.
    Each value in the array will represent a cubie starting from the proper face
    in the middle and extending to the adjacent cubies. A 2x2x2 would look like:
    07  08  24  23  00  00
    05  06  22  21  00  00
    11  09  01  02  14  16
    12  10  03  04  13  15
    00  00  17  18  00  00
    00  00  19  20  00  00

    Y-Axis rotations can be performed by rotating both arrays
    X-Axis rotations can be performed by:
        Popping B int it's own array
*/

/*  [NOTE] Concept of how this will work
    An array of positions will be generated. These positions will identify cubies
    from top-left to bottom-right and in the order of faces U, D, L, R, F, B

    To rotate a face we create a 2D array of the cubie identifiers:
    1  2  3         7  4  1
    4  5  6  --->   8  5  2
    7  8  9         9  6  3

    We also want to include the cubie identifiers for cubies surrounding that
    face in magnitude equal to the number of slices to be rotated with cubie
    identifier of 0 for array buffers:
    00  48  47  46  00
    19  01  02  03  30
    20  04  05  06  29
    21  07  08  09  28
    00  37  38  39  00
*/

// [TODO] Move this out of Global. Only deserves to be here and nowhere else
/*let UpStart =    9 * 0
let DownStart =  9 * 1
let LeftStart =  9 * 2
let RightStart = 9 * 3
let FrontStart = 9 * 4
let BackStart =  9 * 5*/

// [TODO] Fix space/tab issues in this file
class CubeSimulator {
    init(magnitude: Int) {
	    self.magnitude = magnitude
	    generateTranslationTables()
    }

    // Translation Tables will be 1D and in this serialized format:
    // [U Layer 1] ... [U Layer n] [D Layer 1] ... ... [B Layer n]
    // * Note: [X Layer n] is equivalant to an axis rotation about face X
    static func generateTranslationTables(n: Int) {
        // Passed array must be n x n
        // We can rotate the array +90 by transposing it and reversing the rows as shown here:
        //  http://stackoverflow.com/questions/42519/how-do-you-rotate-a-two-dimensional-array?page=1&tab=votes#tab-top
        //  http://stackoverflow.com/questions/21920939/reverse-the-rows-of-a-2d-array
        // [TODO] Make sure this works, not sure if it does
        // [TODO] Make this an array extension instead of a function.
        //        This way we don't have to copy the array every time too
        func transpose(array: Any[][], n: Int) -> return Any[][] {
            retVal = array

            for i in 0 ..< n {
                for j in 0 .. < i {
                    let temp = retVal[i][j]
                    retVal[i][j] = retVal[j][i]
                    retVal[j][i] = temp
                }
            }

            return retVal
        }

        // [TODO] Allow mirroring around Y axis as well as X. Currently only mirrors around X axis
        func mirror(array: Any[][], n: Int) -> return Any[][] {
            retVal = array

            for i in 0 ..< n {
                for j in 0 ..< (n / 2) {
                    let temp = retVal[i][j]
                    retVal[i][j] = retVal[i][n - j - 1]
                    retVal[i][n - j - 1] = temp
                }
            }

            return retVal
        }

        func rotate(array: Any[][], n: Int, count: Int) -> return Any[][] {
            var retVal = array

            for d in 0 ..< count {
                retVal = transpose(retVal, n)
                retVal = mirror(retVal, n)
            }

            return retVal
        }

        var U: [Int][Int](count: n * n, repeatedValue: 0)
        var D: [Int][Int](count: n * n, repeatedValue: 0)
        var L: [Int][Int](count: n * n, repeatedValue: 0)
        var R: [Int][Int](count: n * n, repeatedValue: 0)
        var F: [Int][Int](count: n * n, repeatedValue: 0)
        var B: [Int][Int](count: n * n, repeatedValue: 0)
        let U_startIndex = (n * n) * 0
        let D_startIndex = (n * n) * 1
        let L_startIndex = (n * n) * 2
        let R_startIndex = (n * n) * 3
        let F_startIndex = (n * n) * 4
        let B_startIndex = (n * n) * 5
        for y in 0 ..< n {
            for x in 0 ..< n {
                U[x][y] = (x * y + x) + U_startIndex
                D[x][y] = (x * y + x) + D_startIndex
                L[x][y] = (x * y + x) + L_startIndex
                R[x][y] = (x * y + x) + R_startIndex
                F[x][y] = (x * y + x) + F_startIndex
                B[x][y] = (x * y + x) + B_startIndex
            }
        }

        let faces = ["U": U, "D": D, "L": L, "R": R, "F": F, "B": B]

        // Now we need to concatenate these arrays into their proper axis manipulations
        // These manipulations have been studied and generated outside of this code
        // manipulations will be strings of three characters in length:
        //    manipulation[0] = U, D, L, R, F, B
        //    manipulation[1] = How many rotation are needed. 0, 1, 2, 3
        //    manipulation[2] = If a mirror is needed. 0, 1
        //    (rotations are calculated before mirrors)
        // [TODO] Consider replacing manipulations with tuples instead of formatted strings
        func generateStitchedTranslationTable(faces: [String: [Int][Int]], axisManipulations: [String][String], n: Int) -> [Int][Int] {
            var retVal: [Int][Int](count: (n * n) * 9, repeatedValue: 0)

            for yBlock in 0 ..< 3 {
                for xBlock in 0 ..< 3 {
                    let manipulation = axisManipulations[xBlock][yBlock]
                    let face = faces[manipulation[0]]
                    let rotations = Int(manipulation[1])
                    let mirror = Bool(manipulation[2])

                    for d in rotations {
                        face = rotate(face, n)
                    }

                    if mirror {
                        face = mirror(face, n)
                    }

                    for y in 0 ..< n {
                        for x in 0 ..< n {
                            retVal[(xBlock * n) + x][(yBlock * n) + y] = face[x][y]
                        }
                    }
                }
            }

            return retVal
        }

        let U_AxisManipulations = [["D01", "B20", "D01"],
                                   ["L10", "U00", "R30"],
                                   ["D01", "F00", "D01"]]
        let U_TranslationTable = generateStitchedTranslationTable(faces, U_AxisManipulations)
        let U_AxisTranslationTable = rotate(U_TranslationTable)

        // Now we just need to turn the 2D array into a 1D array
        // [TODO] Turn this into an array extension rather than do it logically here
        func array2Dto1D(array: [Int][Int], n: Int) {
            var retVal = [Int]()
            for y in n {
                for x in n {
                    retVal.append(array[x][y])
                }
            }

            return retVal
        }

        // Now we need to generate a table for each layer that can be rotated

        // We need to make sure we set the array length to (n * n) * 3 since we stitched a 3x3 grid of face arrays
        var U_TranslationTable_1D = array2Dto1D(U_TranslationTable, (n * n) * 3)
        var U_AxisTranslationTable_1D = array2Dto1D(U_AxisTranslationTable, (n * n) * 3)

        // BEGIN ACTUAL TESTING
        // From here we are going to try and apply the translation table to a cubie array and see if it works
    }

    /* END OF FINAL ATTEMPT TO REFACTOR CubeSimulator */

    /*init(magnitude: Int) {
	    self.magnitude = magnitude
	    reset()
    }

    // [TODO] Do I really need this method? Isn't it stupid to convert to string, and then back again?
    // if I'm just managing everything in this class then we can pass inhouse Int[][] instead of formatted strings
    func faceTranslationString(translations: [Int]) -> String {
       retVal = ""
        for d in 0 ..< translations.count {
            Bool shouldPrefixDelimiter = true
            if d == 0 {shouldPrefixDelimiter = false}

            if shouldPrefixDelimiter {retVal += ":"}
            retVal += String(format: "%i", translations[d])
        }

        return retVal
    }

    // Rotaton only accounts for U face. All other rotations will be generated by axis rotations, execute U, then axis rotation prime
    // Even though the D face is never manipulated I'm still going to include it
    // The numbers in those cubies represents what existing cubie index needs to take that index's place
    // We can calculate the other face array translations dynamically instead of by hand
    // Luckily we only have to do this once
    func calculateFull_U_Translation(UFace_Translation: String) -> String {
        // Faces in the array are represented from top-left to bottom-right ordered by integers
        // Faces in the array are then ordered U, D, L, R, F, B
        retVal = UFace_Translation

        let baseCubieIndex_U = 0
        let baseCubieIndex_D = baseCubieIndex_U + (magnitude * magnitude)
        let baseCubieIndex_L = baseCubieIndex_D + (magnitude * magnitude)
        let baseCubieIndex_R = baseCubieIndex_L + (magnitude * magnitude)
        let baseCubieIndex_F = baseCubieIndex_R + (magnitude * magnitude)
        let baseCubieIndex_B = baseCubieIndex_F + (magnitude * magnitude)

        // First let's handle the D face translation. This one is the easiest since there are no translations
        let DFace_Translations = [Int]()
        for d in 0 ..< (magnitude * magnitude) {
            DFace_Translations.append(baseCubieIndex_D + d)
        }
        retVal += faceTranslationString(DFace_Translations)

        // Now we handle the L face translation. Things get harder here
        let LFace_Translations = [Int]()
        for d in 0 ..< magnitude {
            LFace_Translations.append(baseCubieIndexB + d)
        }
        retVal += faceTranslationString(LFace_Translations)

        // Handle R face translation
        let RFace_Translations = [Int]()
        for d in 0 ..< magnitude {
            RFace_Translations.append(baseCubieIndexF + d)
        }
        retVal += faceTranslationString(RFace_Translations)

        // Handle F face translation
        let FFace_Translations = [Int]()
        for d in 0 ..< magnitude {
            FFace_Translations.append(baseCubieIndexL + d)
        }
        retVal += faceTranslationString(FFace_Translations)

        // Handle B face translation
        let BFace_Translations = [Int]()
        for d in 0 ..< magnitude {
            BFace_Translations.append(baseCubieIndexR + d)
        }
        retVal += faceTranslationString(BFace_Translations)

        return retVal
    }

    // [TODO] Do I really need this?
    //func colorOfCubie(index: Int) -> Int {
    //    return cubies[index]
    //}

    func reset() {
        for d in 0 ..< 9 {
            cubies[UpStart    + d] = k.faces.up
            cubies[DownStart  + d] = k.faces.down
            cubies[LeftStart  + d] = k.faces.left
            cubies[RightStart + d] = k.faces.right
            cubies[FrontStart + d] = k.faces.front
            cubies[BackStart  + d] = k.faces.back
        }
    }

    func performMove(move: String) {
        if move == k.moves.U {
            //
        }
    }*/
    // [TODO] Determine if I want it to be move: String or move: Int and then place them in Global
    /*func performMove(move: String) {
        var newCubies = cubies

        if move == k.moves.U {
		    newCubies[UpStart+0] = cubies[UpStart+6];
		    newCubies[UpStart+1] = cubies[UpStart+3];
		    newCubies[UpStart+2] = cubies[UpStart+0];
		    newCubies[UpStart+3] = cubies[UpStart+7];
		    newCubies[UpStart+4] = cubies[UpStart+4];
		    newCubies[UpStart+5] = cubies[UpStart+1];
		    newCubies[UpStart+6] = cubies[UpStart+8];
		    newCubies[UpStart+7] = cubies[UpStart+5];
		    newCubies[UpStart+8] = cubies[UpStart+2];

		    newCubies[FrontStart+0] = cubies[RightStart+0];
		    newCubies[FrontStart+1] = cubies[RightStart+1];
		    newCubies[FrontStart+2] = cubies[RightStart+2];

		    newCubies[LeftStart+0] = cubies[FrontStart+0];
		    newCubies[LeftStart+1] = cubies[FrontStart+1];
		    newCubies[LeftStart+2] = cubies[FrontStart+2];

		    newCubies[BackStart+0] = cubies[LeftStart+0];
		    newCubies[BackStart+1] = cubies[LeftStart+1];
		    newCubies[BackStart+2] = cubies[LeftStart+2];

		    newCubies[RightStart+0] = cubies[BackStart+0];
		    newCubies[RightStart+1] = cubies[BackStart+1];
		    newCubies[RightStart+2] = cubies[BackStart+2];
		    for(int d=0;d<54;d++)cubies[d] = newCubies[d];
	    }
	    else if([move isEqualToString:@"U2"]) {
    		for(int d=0;d<2;d++)[self performMove:@"U"];
    	}
    	else if([move isEqualToString:@"U'"]) {
		    for(int d=0;d<3;d++)[self performMove:@"U"];
	    }
	    else if([move isEqualToString:@"D"]) {
    		newCubies[DownStart+0] = cubies[DownStart+6];
		    newCubies[DownStart+1] = cubies[DownStart+3];
		    newCubies[DownStart+2] = cubies[DownStart+0];
		    newCubies[DownStart+3] = cubies[DownStart+7];
		    newCubies[DownStart+4] = cubies[DownStart+4];
		    newCubies[DownStart+5] = cubies[DownStart+1];
		    newCubies[DownStart+6] = cubies[DownStart+8];
		    newCubies[DownStart+7] = cubies[DownStart+5];
		    newCubies[DownStart+8] = cubies[DownStart+2];

		    newCubies[FrontStart+6] = cubies[LeftStart+6];
		    newCubies[FrontStart+7] = cubies[LeftStart+7];
		    newCubies[FrontStart+8] = cubies[LeftStart+8];

		    newCubies[LeftStart+6] = cubies[BackStart+6];
		    newCubies[LeftStart+7] = cubies[BackStart+7];
		    newCubies[LeftStart+8] = cubies[BackStart+8];

		    newCubies[BackStart+6] = cubies[RightStart+6];
		    newCubies[BackStart+7] = cubies[RightStart+7];
		    newCubies[BackStart+8] = cubies[RightStart+8];

		    newCubies[RightStart+6] = cubies[FrontStart+6];
		    newCubies[RightStart+7] = cubies[FrontStart+7];
		    newCubies[RightStart+8] = cubies[FrontStart+8];
		    for(int d=0;d<54;d++)cubies[d] = newCubies[d];
	    }
	    else if([move isEqualToString:@"D2"]) {
    		for(int d=0;d<2;d++)[self performMove:@"D"];
    	}
    	else if([move isEqualToString:@"D'"]) {
		    for(int d=0;d<3;d++)[self performMove:@"D"];
	    }
	    else if([move isEqualToString:@"L"]) {
    		newCubies[LeftStart+0] = cubies[LeftStart+6];
		    newCubies[LeftStart+1] = cubies[LeftStart+3];
		    newCubies[LeftStart+2] = cubies[LeftStart+0];
		    newCubies[LeftStart+3] = cubies[LeftStart+7];
		    newCubies[LeftStart+4] = cubies[LeftStart+4];
		    newCubies[LeftStart+5] = cubies[LeftStart+1];
		    newCubies[LeftStart+6] = cubies[LeftStart+8];
		    newCubies[LeftStart+7] = cubies[LeftStart+5];
		    newCubies[LeftStart+8] = cubies[LeftStart+2];

		    newCubies[UpStart+0] = cubies[BackStart+8];
		    newCubies[UpStart+3] = cubies[BackStart+5];
		    newCubies[UpStart+6] = cubies[BackStart+2];

		    newCubies[FrontStart+0] = cubies[UpStart+0];
		    newCubies[FrontStart+3] = cubies[UpStart+3];
		    newCubies[FrontStart+6] = cubies[UpStart+6];

		    newCubies[DownStart+0] = cubies[FrontStart+0];
		    newCubies[DownStart+3] = cubies[FrontStart+3];
		    newCubies[DownStart+6] = cubies[FrontStart+6];

		    newCubies[BackStart+2] = cubies[DownStart+6];
		    newCubies[BackStart+5] = cubies[DownStart+3];
		    newCubies[BackStart+8] = cubies[DownStart+0];
		    for(int d=0;d<54;d++)cubies[d] = newCubies[d];
	    }
	    else if([move isEqualToString:@"L2"]) {
    		for(int d=0;d<2;d++)[self performMove:@"L"];
    	}
    	else if([move isEqualToString:@"L'"]) {
		    for(int d=0;d<3;d++)[self performMove:@"L"];
	    }
	    else if([move isEqualToString:@"R"]) {
    		newCubies[RightStart+0] = cubies[RightStart+6];
		    newCubies[RightStart+1] = cubies[RightStart+3];
		    newCubies[RightStart+2] = cubies[RightStart+0];
		    newCubies[RightStart+3] = cubies[RightStart+7];
		    newCubies[RightStart+4] = cubies[RightStart+4];
		    newCubies[RightStart+5] = cubies[RightStart+1];
		    newCubies[RightStart+6] = cubies[RightStart+8];
		    newCubies[RightStart+7] = cubies[RightStart+5];
		    newCubies[RightStart+8] = cubies[RightStart+2];

		    newCubies[UpStart+2] = cubies[FrontStart+2];
		    newCubies[UpStart+5] = cubies[FrontStart+5];
		    newCubies[UpStart+8] = cubies[FrontStart+8];

		    newCubies[FrontStart+2] = cubies[DownStart+2];
		    newCubies[FrontStart+5] = cubies[DownStart+5];
		    newCubies[FrontStart+8] = cubies[DownStart+8];

		    newCubies[DownStart+2] = cubies[BackStart+6];
		    newCubies[DownStart+5] = cubies[BackStart+3];
		    newCubies[DownStart+8] = cubies[BackStart+0];

		    newCubies[BackStart+6] = cubies[UpStart+2];
		    newCubies[BackStart+3] = cubies[UpStart+5];
		    newCubies[BackStart+0] = cubies[UpStart+8];
		    for(int d=0;d<54;d++)cubies[d] = newCubies[d];
	    }
	    else if([move isEqualToString:@"R2"]) {
    		for(int d=0;d<2;d++)[self performMove:@"R"];
    	}
    	else if([move isEqualToString:@"R'"]) {
		    for(int d=0;d<3;d++)[self performMove:@"R"];
	    }
	    else if([move isEqualToString:@"F"]) {
    		newCubies[FrontStart+0] = cubies[FrontStart+6];
		    newCubies[FrontStart+1] = cubies[FrontStart+3];
		    newCubies[FrontStart+2] = cubies[FrontStart+0];
		    newCubies[FrontStart+3] = cubies[FrontStart+7];
		    newCubies[FrontStart+4] = cubies[FrontStart+4];
		    newCubies[FrontStart+5] = cubies[FrontStart+1];
		    newCubies[FrontStart+6] = cubies[FrontStart+8];
		    newCubies[FrontStart+7] = cubies[FrontStart+5];
		    newCubies[FrontStart+8] = cubies[FrontStart+2];

		    newCubies[UpStart+6] = cubies[LeftStart+8];
		    newCubies[UpStart+7] = cubies[LeftStart+5];
		    newCubies[UpStart+8] = cubies[LeftStart+2];

		    newCubies[LeftStart+2] = cubies[DownStart+0];
		    newCubies[LeftStart+5] = cubies[DownStart+1];
		    newCubies[LeftStart+8] = cubies[DownStart+2];

		    newCubies[DownStart+0] = cubies[RightStart+6];
		    newCubies[DownStart+1] = cubies[RightStart+3];
		    newCubies[DownStart+2] = cubies[RightStart+0];

		    newCubies[RightStart+0] = cubies[UpStart+6];
		    newCubies[RightStart+3] = cubies[UpStart+7];
		    newCubies[RightStart+6] = cubies[UpStart+8];
		    for(int d=0;d<54;d++)cubies[d] = newCubies[d];
	    }
	    else if([move isEqualToString:@"F2"]) {
    		for(int d=0;d<2;d++)[self performMove:@"F"];
    	}
    	else if([move isEqualToString:@"F'"]) {
		    for(int d=0;d<3;d++)[self performMove:@"F"];
	    }
	    else if([move isEqualToString:@"B"]) {
    		newCubies[BackStart+0] = cubies[BackStart+6];
		    newCubies[BackStart+1] = cubies[BackStart+3];
		    newCubies[BackStart+2] = cubies[BackStart+0];
		    newCubies[BackStart+3] = cubies[BackStart+7];
		    newCubies[BackStart+4] = cubies[BackStart+4];
		    newCubies[BackStart+5] = cubies[BackStart+1];
		    newCubies[BackStart+6] = cubies[BackStart+8];
		    newCubies[BackStart+7] = cubies[BackStart+5];
		    newCubies[BackStart+8] = cubies[BackStart+2];

		    newCubies[UpStart+0] = cubies[RightStart+2];
		    newCubies[UpStart+1] = cubies[RightStart+5];
		    newCubies[UpStart+2] = cubies[RightStart+8];

		    newCubies[RightStart+2] = cubies[DownStart+8];
		    newCubies[RightStart+5] = cubies[DownStart+7];
		    newCubies[RightStart+8] = cubies[DownStart+6];

		    newCubies[DownStart+8] = cubies[LeftStart+6];
		    newCubies[DownStart+7] = cubies[LeftStart+3];
		    newCubies[DownStart+6] = cubies[LeftStart+0];

		    newCubies[LeftStart+0] = cubies[UpStart+2];
		    newCubies[LeftStart+3] = cubies[UpStart+1];
		    newCubies[LeftStart+6] = cubies[UpStart+0];
		    for(int d=0;d<54;d++)cubies[d] = newCubies[d];
	    }
    	else if([move isEqualToString:@"B2"]) {
		    for(int d=0;d<2;d++)[self performMove:@"B"];
	    }
	    else if([move isEqualToString:@"B'"]) {
    		for(int d=0;d<3;d++)[self performMove:@"B"];
    	}
    }*/

    /*- (void)performAlgorithm:(NSString *)algorithm {
    	NSArray *moveSet = [algorithm componentsSeparatedByString:@" "];

	    for(NSString *move in moveSet)
    		[self performMove:move];
    }

    - (void)log {
    	NSLog(@"Up");
    	for(int d=UpStart;d<UpStart+9;d++) {
		    if(cubies[d] == COLOR_UP)NSLog(@"W");
		    else if(cubies[d] == COLOR_DOWN)NSLog(@"Y");
		    else if(cubies[d] == COLOR_LEFT)NSLog(@"O");
		    else if(cubies[d] == COLOR_RIGHT)NSLog(@"R");
		    else if(cubies[d] == COLOR_FRONT)NSLog(@"G");
		    else if(cubies[d] == COLOR_BACK)NSLog(@"B");
	    }
	    NSLog(@"Down");
	    for(int d=DownStart;d<DownStart+9;d++) {
    		if(cubies[d] == COLOR_UP)NSLog(@"W");
		    else if(cubies[d] == COLOR_DOWN)NSLog(@"Y");
		    else if(cubies[d] == COLOR_LEFT)NSLog(@"O");
		    else if(cubies[d] == COLOR_RIGHT)NSLog(@"R");
		    else if(cubies[d] == COLOR_FRONT)NSLog(@"G");
		    else if(cubies[d] == COLOR_BACK)NSLog(@"B");
	    }
	    NSLog(@"Left");
	    for(int d=LeftStart;d<LeftStart+9;d++) {
    		if(cubies[d] == COLOR_UP)NSLog(@"W");
		    else if(cubies[d] == COLOR_DOWN)NSLog(@"Y");
		    else if(cubies[d] == COLOR_LEFT)NSLog(@"O");
		    else if(cubies[d] == COLOR_RIGHT)NSLog(@"R");
		    else if(cubies[d] == COLOR_FRONT)NSLog(@"G");
		    else if(cubies[d] == COLOR_BACK)NSLog(@"B");
	    }
	    NSLog(@"Right");
	    for(int d=RightStart;d<RightStart+9;d++) {
    		if(cubies[d] == COLOR_UP)NSLog(@"W");
		    else if(cubies[d] == COLOR_DOWN)NSLog(@"Y");
		    else if(cubies[d] == COLOR_LEFT)NSLog(@"O");
		    else if(cubies[d] == COLOR_RIGHT)NSLog(@"R");
		    else if(cubies[d] == COLOR_FRONT)NSLog(@"G");
		    else if(cubies[d] == COLOR_BACK)NSLog(@"B");
	    }
	    NSLog(@"Front");
	    for(int d=FrontStart;d<FrontStart+9;d++) {
    		if(cubies[d] == COLOR_UP)NSLog(@"W");
		    else if(cubies[d] == COLOR_DOWN)NSLog(@"Y");
		    else if(cubies[d] == COLOR_LEFT)NSLog(@"O");
		    else if(cubies[d] == COLOR_RIGHT)NSLog(@"R");
		    else if(cubies[d] == COLOR_FRONT)NSLog(@"G");
		    else if(cubies[d] == COLOR_BACK)NSLog(@"B");
	    }
	    NSLog(@"Back");
	    for(int d=BackStart;d<BackStart+9;d++) {
    		if(cubies[d] == COLOR_UP)NSLog(@"W");
		    else if(cubies[d] == COLOR_DOWN)NSLog(@"Y");
		    else if(cubies[d] == COLOR_LEFT)NSLog(@"O");
		    else if(cubies[d] == COLOR_RIGHT)NSLog(@"R");
		    else if(cubies[d] == COLOR_FRONT)NSLog(@"G");
		    else if(cubies[d] == COLOR_BACK)NSLog(@"B");
	    }
    }

    // 0 for non white, 1 for white. Start upper left of cubies to bottom right, then upper left cubie back clockwise to upper left cubie
    - (NSString *)OLLHitTestValue {
    	char list[22];
    	for(int d=0;d<21;d++)list[d] = '0';

	    for(int d=0;d<9;d++) {
    		if(cubies[UpStart+d] == COLOR_UP)list[d] = '1';
    	}

    	if(cubies[BackStart+2] == COLOR_UP)list[9] = '1';
    	if(cubies[BackStart+1] == COLOR_UP)list[10] = '1';
    	if(cubies[BackStart+0] == COLOR_UP)list[11] = '1';

	    if(cubies[RightStart+2] == COLOR_UP)list[12] = '1';
	    if(cubies[RightStart+1] == COLOR_UP)list[13] = '1';
	    if(cubies[RightStart+0] == COLOR_UP)list[14] = '1';

	    if(cubies[FrontStart+2] == COLOR_UP)list[15] = '1';
	    if(cubies[FrontStart+1] == COLOR_UP)list[16] = '1';
	    if(cubies[FrontStart+0] == COLOR_UP)list[17] = '1';

	    if(cubies[LeftStart+2] == COLOR_UP)list[18] = '1';
	    if(cubies[LeftStart+1] == COLOR_UP)list[19] = '1';
	    if(cubies[LeftStart+0] == COLOR_UP)list[20] = '1';

	    list[21] = '\0';

	    NSString *retVal = [NSString stringWithCString:list encoding:NSASCIIStringEncoding];
	    return retVal;
    }

    // b for back color, r for right color, f for front color, l for left color
    - (NSString *)PLLHitTestValue {
    	char list[13];

    	list[0] = cubies[BackStart+2];
    	list[1] = cubies[BackStart+1];
    	list[2] = cubies[BackStart+0];

    	list[3] = cubies[RightStart+2];
    	list[4] = cubies[RightStart+1];
    	list[5] = cubies[RightStart+0];

    	list[6] = cubies[FrontStart+2];
    	list[7] = cubies[FrontStart+1];
    	list[8] = cubies[FrontStart+0];

    	list[9] = cubies[LeftStart+2];
    	list[10] = cubies[LeftStart+1];
    	list[11] = cubies[LeftStart+0];

    	for(int d=0;d<12;d++) {
		    if(FALSE) {}
		    else if(list[d] == COLOR_BACK)list[d] = 'b';
		    else if(list[d] == COLOR_RIGHT)list[d] = 'r';
		    else if(list[d] == COLOR_FRONT)list[d] = 'f';
		    else if(list[d] == COLOR_LEFT)list[d] = 'l';
	    }

	    list[12] = '\0';

	    NSString *retVal = [NSString stringWithCString:list encoding:NSASCIIStringEncoding];
	    return retVal;
    }

    var _UFace_Translation: String?
    var UFace_Translation {
        get {
            if _UFace_Translation == nil {
                if magnitude == 2 {
                    // [TODO] Verify this UFace rotation is correct
                    _UFace_Translation = calculateFull_U_Translation("2:0:3:1")
                }
                else if magnitude == 3 {
                    // [TODO] Verify this UFace rotation is correct
                    _UFace_Translation = calculateFull_U_Translation("4:1:2:7:5:3:8:9:6")
                }
                else {
                    print("[ERROR] Unable to process UFace_Translation with current magnitude")
                }
            }

            return _UFace_Translation!
        }
        set(newVal) {
            if newVal != nil {
                print("[ERROR] Unable to do anything but set UFace_Translation to nil")
            }
            else {
                _UFace_Translation = newVal
            }
        }
    }*/

    var magnitude = 3
//    var cubies = [Int](count: 54, repeatedValue: 0)
}
