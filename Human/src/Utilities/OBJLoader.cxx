#include "./Human/include/Utilities/OBJLoader.h"
#include "./Core/include/OPlog.h"
#include "./Human/include/Resources/Buffer/OPrenderBuffer.h"
#include "./Human/include/Resources/Model/OPmesh.h"
#include "./Human/include/Rendering/OPMvertex.h"

// FacePoint contains 3 int's being holding points for
// the index's for each array.
typedef struct
{
	int VertexIndex;
	int TextureIndex;
	int NormalIndex;
} FacePoint;

// Face contains either 3 or 4 FacePoints, 
// tri is marked as true if it's a triangle
typedef struct
{
	FacePoint one;
	FacePoint two;
	FacePoint three;
	FacePoint four;
	bool tri;
} Face;

int readLine(char* buffer, FILE* file){
	int length = 0;
	char ch = ' ';
	while(ch != '\n'){
		fread(&ch,1, 1, file);
		buffer[length] = ch;
		length++;
	}
	return length;
}

void GenerateTangent(OPvec3* tangent, OPMvertex* v1, OPMvertex* v2){
	f32 dx = v1->vertice.x - v2->vertice.x;
	f32 dy = v1->vertice.y - v2->vertice.y;
	f32 dz = v1->vertice.z - v2->vertice.z;

	OPvec3 diff;
	diff.x = dx;
	diff.y = dy;
	diff.z = dz;
	OPvec3 tan;
	OPvec3cross(tan, v1->normal, diff);
	OPvec3norm(tan, tan);
	tangent->x = tan.x;
	tangent->y = tan.y;
	tangent->z = tan.z;
}

void SetFaceData(OPMvertex* vertex, FacePoint* facePoint, OPvec3* vertexes, OPvec2* texes, OPvec3* normals){
	vertex->vertice.x = vertexes[facePoint->VertexIndex - 1].x;
	vertex->vertice.y = vertexes[facePoint->VertexIndex - 1].y;
	vertex->vertice.z = vertexes[facePoint->VertexIndex - 1].z;

	vertex->tex.x = texes[facePoint->TextureIndex - 1].x;
	vertex->tex.y = texes[facePoint->TextureIndex - 1].y;

	vertex->normal.x = normals[facePoint->NormalIndex - 1].x;
	vertex->normal.y = normals[facePoint->NormalIndex - 1].y;
	vertex->normal.z = normals[facePoint->NormalIndex - 1].z;
}

Mesh* LoadOBJ(FILE* file, int start, int length)
{
	//Open File for reading
	char* buffer = (char*)OPalloc(sizeof(char) * 4096);

	char one[20], two[20], three[20], four[20];

	int total_verts = 0;
	int total_texs = 0;
	int total_norms = 0;
	int total_faces = 0;
	
	int offset = 0;
	offset += readLine(buffer, file);
	fseek(file, offset, start);
	while(offset < length){
		switch(lineType(buffer)){
			case -1:
				// Ignore it
				break;
			case 0:
				total_verts++;
				break;
			case 1:
				total_texs++;
				break;
			case 2:
				total_norms++;
			case 3:
				total_faces++;
			case 4:
				break;
		}
		offset += readLine(buffer, file);
		fseek(file, offset, start);
	}


	int c_verts = 0;
	int c_texs = 0;
	int c_norms = 0;
	int c_faces = 0;

	OPvec3* vertexes = new OPvec3[total_verts];
	OPbzero(vertexes, sizeof(OPvec3) * total_verts);

	OPvec2* texes = new OPvec2[total_texs];
	OPvec3* normals = new OPvec3[total_norms];
	Face* faces = new Face[total_faces];

	offset = 0;
	fseek(file, start, SEEK_SET);
	
	float x, y, z;
		
	unsigned int ind1, ind2, ind3, ind4, ind5, ind6, ind7, ind8, ind9, ind10, ind11, ind12, ind13, ind14;

	offset += readLine(buffer, file);
	fseek(file, offset, start);
	int t1;
	
	int i = 0, cnt = 0;
	while(offset < length){
		t1 = lineType(buffer);
		switch(t1){
			case -1:
				// Ignore it
				break;
			case 0: //v		Vertex
				if(sscanf(buffer, "v %f %f %f", &x, &y, &z) == 3){
					vertexes[c_verts].x = x;
					vertexes[c_verts].y = y;
					vertexes[c_verts].z = z;

					c_verts++;
				}
				break;
			case 1: //vt	Texture Coordinate
				if(sscanf(buffer, "vt %f %f", &x, &y) == 2){
					texes[c_texs].x = x;
					texes[c_texs].y = -y + 1;
					c_texs++;
				}
				break;
			case 2: //vn	Normal
				if(sscanf(buffer, "vn %f %f %f", &x, &y, &z) == 3){
					normals[c_norms].x = x;
					normals[c_norms].y = y;
					normals[c_norms].z = z;
					c_norms++;
				}
				break;
			case 3: //f		Face
				
				i = 0;
				cnt = 0;
				do {

					i++;
					if (buffer[i] == ' ')
						cnt++;

				} while ((buffer[i] != '\0') && (buffer[i] != '\n'));

				if (cnt == 3)
				{
					if (sscanf(buffer, "f %u/%u/%u %u/%u/%u %u/%u/%u", &ind1, &ind2, &ind3, &ind4, &ind5, &ind6, &ind7, &ind8, &ind9) == 9)
					{
						faces[c_faces].one.VertexIndex = ind1;
						faces[c_faces].one.TextureIndex = ind2;
						faces[c_faces].one.NormalIndex = ind3;
						
						faces[c_faces].two.VertexIndex = ind4;
						faces[c_faces].two.TextureIndex = ind5;
						faces[c_faces].two.NormalIndex = ind6;

						faces[c_faces].three.VertexIndex = ind7;
						faces[c_faces].three.TextureIndex = ind8;
						faces[c_faces].three.NormalIndex = ind9;

						faces[c_faces].tri = true;
						c_faces++;
					}
				}
				else if (cnt == 4)
				{
					if (sscanf(buffer, "f %u/%u/%u %u/%u/%u %u/%u/%u %u/%u/%u", &ind1, &ind2, &ind3, &ind4, &ind5, &ind6, &ind7, &ind8, &ind9, &ind10, &ind11, &ind12) == 12)
					{
						faces[c_faces].one.VertexIndex = ind1;
						faces[c_faces].one.TextureIndex = ind2;
						faces[c_faces].one.NormalIndex = ind3;
						
						faces[c_faces].two.VertexIndex = ind4;
						faces[c_faces].two.TextureIndex = ind5;
						faces[c_faces].two.NormalIndex = ind6;

						faces[c_faces].three.VertexIndex = ind7;
						faces[c_faces].three.TextureIndex = ind8;
						faces[c_faces].three.NormalIndex = ind9;

						faces[c_faces].four.VertexIndex = ind10;
						faces[c_faces].four.TextureIndex = ind11;
						faces[c_faces].four.NormalIndex = ind12;

						faces[c_faces].tri = false;
						c_faces++;
					}
				}
				break;
			case 4: // material

				break;
		}
		offset += readLine(buffer, file);
		fseek(file, offset, start);
	}

	int place = 0;
	int count = c_faces;
	int pointCount = 0;

	int totalPoints = 0;
	int totalIndices = 0;
	for(i = 0; i < c_faces; i++){
		if(faces[i].tri){
			totalPoints += 3;
			totalIndices += 3;
		}
		else{
			totalPoints += 4;
			totalIndices += 6;
		}
	}
	
	
	OPMvertex* points = (OPMvertex*)OPalloc(sizeof(OPMvertex) * totalPoints);
	unsigned short* indices = (unsigned short*)OPalloc(sizeof(unsigned short) * totalIndices);
	int currPoint = 0;
	int currIndex = 0;

	for(i = 0; i < count; i++)
	{
		SetFaceData( &points[currPoint], &faces[i].one, vertexes, texes, normals);
		currPoint++;	

		SetFaceData( &points[currPoint], &faces[i].two, vertexes, texes, normals);
		currPoint++;	

		SetFaceData( &points[currPoint], &faces[i].three, vertexes, texes, normals);
		currPoint++;	

		//Face Point Four
		//Only used if it's a quad, and not a triangle
		if(!faces[i].tri)
		{		
			SetFaceData( &points[currPoint], &faces[i].four, vertexes, texes, normals);
			currPoint++;	
		}

		//Push indices into the indices list. .obj file
		//creates triangles by (p1,p2,p3) for triangles
		//and (p1,p2,p3) (p1,p3,p4) for quads.
		indices[currIndex++] = place;
		indices[currIndex++] = place + 1;
		indices[currIndex++] = place + 2;
		if(!faces[i].tri)
		{
			indices[currIndex++] = place;
			indices[currIndex++] = place + 2;
			indices[currIndex++] = place + 3;
			place++;
		}
		place += 3;
	}

	OPMvertex* vert_one;
	OPMvertex* vert_two;
	OPvec3* tangent;
	for(i = 0; i < currIndex; i+=3){
		vert_one = &points[indices[i] + 0];
		vert_two = &points[indices[i] + 1];
		GenerateTangent(&vert_one->tangent, vert_one, vert_two);
		
		vert_one = &points[indices[i] + 1];
		vert_two = &points[indices[i] + 2];
		GenerateTangent(&vert_one->tangent, vert_one, vert_two);
		
		vert_one = &points[indices[i] + 2];
		vert_two = &points[indices[i] + 0];
		GenerateTangent(&vert_one->tangent, vert_one, vert_two);
	}

	delete[] vertexes;
	delete[] texes;
	delete[] normals;
	OPfree(buffer); // TODO any reason you're using OPfree here, but new and delete everywhere else?

	BufferPtr vertexBuffer = new Buffer(VertexBuffer, sizeof(OPMvertex) * totalPoints, points);
	BufferPtr indexBuffer = new Buffer(IndexBuffer, sizeof(unsigned short) * totalIndices, indices);
	
	Mesh* out = new Mesh(vertexBuffer, indexBuffer, totalIndices, sizeof(OPMvertex));

	OPfree(points);
	OPfree(indices);

	return out;
}

int lineType(char* word)
{
	if(word[0] == 'v') //Vertex
	{
		if(word[1] == ' ') //Vertex Point
			return 0;
		if(word[1] == 't') //Vertex Texture Coordinate
			return 1;
		if(word[1] == 'n') //Vertex Normal
			return 2;
	}
	if(word[0] == 'f') //Face
		return 3;
	if(word[0] == 't') //Face
		return 4;
	return -1; //(comment '#')
}