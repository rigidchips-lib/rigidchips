xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 22;
 0.00000;0.01000;-0.10000;,
 0.00000;-0.01000;-0.10000;,
 0.09000;-0.01000;-0.27000;,
 0.09000;0.01000;-0.27000;,
 -0.09000;0.01000;-0.27000;,
 -0.09000;-0.01000;-0.27000;,
 -0.26973;0.00999;0.26973;,
 -0.29970;0.00999;-0.29970;,
 -0.29970;0.00999;0.29970;,
 0.29970;0.00999;0.29970;,
 -0.26973;0.00999;-0.26973;,
 0.29970;0.00999;-0.29970;,
 0.26973;0.00999;-0.26973;,
 0.26973;0.00999;0.26973;,
 -0.26973;-0.00999;0.26973;,
 0.29970;-0.00999;0.29970;,
 -0.29970;-0.00999;0.29970;,
 -0.29970;-0.00999;-0.29970;,
 0.26973;-0.00999;0.26973;,
 0.29970;-0.00999;-0.29970;,
 0.26973;-0.00999;-0.26973;,
 -0.26973;-0.00999;-0.26973;;
 
 36;
 4;0,1,2,3;,
 4;1,0,4,5;,
 3;0,3,4;,
 3;1,5,2;,
 3;6,7,8;,
 3;9,6,8;,
 3;10,7,6;,
 3;10,11,7;,
 3;12,11,10;,
 3;12,9,11;,
 3;13,9,12;,
 3;6,9,13;,
 3;14,15,16;,
 3;17,14,16;,
 3;18,15,14;,
 3;18,19,15;,
 3;20,19,18;,
 3;20,17,19;,
 3;21,17,20;,
 3;14,17,21;,
 3;12,18,13;,
 3;18,12,20;,
 3;13,14,6;,
 3;14,13,18;,
 3;10,20,12;,
 3;20,10,21;,
 3;6,21,10;,
 3;21,6,14;,
 3;16,7,17;,
 3;7,16,8;,
 3;17,11,19;,
 3;11,17,7;,
 3;8,15,9;,
 3;15,8,16;,
 3;11,15,19;,
 3;15,11,9;;
 
 MeshMaterialList {
  1;
  36;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   100.000000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "Trim.dds";
   }
  }
 }
 MeshNormals {
  12;
  0.883788;0.000000;0.467888;,
  -0.883788;0.000000;0.467888;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  -0.000000;0.000000;1.000000;,
  1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;;
  36;
  4;0,0,0,0;,
  4;1,1,1,1;,
  3;3,3,3;,
  3;2,2,2;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;4,4,5;,
  3;4,4,5;,
  3;6,6,7;,
  3;6,6,7;,
  3;8,8,9;,
  3;8,8,9;,
  3;10,10,11;,
  3;10,10,11;,
  3;4,4,5;,
  3;4,4,5;,
  3;6,6,7;,
  3;6,6,7;,
  3;8,8,9;,
  3;8,8,9;,
  3;10,10,11;,
  3;10,10,11;;
 }
 MeshTextureCoords {
  22;
  0.500000;0.333333;
  0.500000;0.333333;
  0.650000;0.050000;
  0.650000;0.050000;
  0.350000;0.050000;
  0.350000;0.050000;
  0.050450;0.949550;
  0.000500;0.000500;
  0.000500;0.999500;
  0.999500;0.999500;
  0.050450;0.050450;
  0.999500;0.000500;
  0.949550;0.050450;
  0.949550;0.949550;
  0.050450;0.949550;
  0.999500;0.999500;
  0.000500;0.999500;
  0.000500;0.000500;
  0.949550;0.949550;
  0.999500;0.000500;
  0.949550;0.050450;
  0.050450;0.050450;;
 }
}
