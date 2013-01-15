#include "OBJLoader.h"
#include ".\Core\include\Log.h"

int readLine(char* buffer, FILE* file){
	int length = 0;
	char ch;
	while(ch != '\n'){
		fread(&ch,1, 1, file);
		buffer[length] = ch;
		length++;
	}
	//OPLog("readLine");
	//OPLog(buffer);
	return length;
}

Mesh* LoadOBJ(FILE* file, int start, int length)
{
	//ObjectMesh to fill
	Mesh* mesh = new Mesh();

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

	OPLog("Loading OBJ");
	OPLog_i32(total_verts);
	OPLog_i32(total_texs);
	OPLog_i32(total_norms);
	OPLog_i32(total_faces);

	int c_verts = 0;
	int c_texs = 0;
	int c_norms = 0;
	int c_faces = 0;

	Vector3* vertexes = new Vector3[total_verts];
	for(int j = 0; j < total_verts; j++)
		vertexes[j] = 0;
	Vector2* texes = new Vector2[total_texs];
	Vector3* normals = new Vector3[total_norms];
	Face* faces = new Face[total_faces];
	
	offset = 0;
	fseek(file, start, SEEK_SET);
	
	float x, y, z;
		
	unsigned int ind1, ind2, ind3, ind4, ind5, ind6, ind7, ind8;

	OPLog("Reading file");
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
					vertexes[c_verts] = Vector3(x, y, z);
					c_verts++;
				}
				break;
			case 1: //vt	Texture Coordinate
				if(sscanf(buffer, "vt %f %f", &x, &y) == 2){
					texes[c_texs] = Vector2(x, -y + 1);
					c_texs++;
				}
				break;
			case 2: //vn	Normal
				if(sscanf(buffer, "vn %f %f %f", &x, &y, &z) == 3){
					normals[c_norms] = Vector3(x, y, z);
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
					if (sscanf(buffer, "f %u/%u %u/%u %u/%u", &ind1, &ind2, &ind3, &ind4, &ind5, &ind6) == 6)
					{
						faces[c_faces].one.VertexIndex = ind1;
						faces[c_faces].one.TextureIndex = ind2;
						
						faces[c_faces].two.VertexIndex = ind3;
						faces[c_faces].two.TextureIndex = ind4;

						faces[c_faces].three.VertexIndex = ind5;
						faces[c_faces].three.TextureIndex = ind6;

						faces[c_faces].tri = true;
						c_faces++;
					}
				}
				else if (cnt == 4)
				{
					if (sscanf(buffer, "f %u/%u %u/%u %u/%u %u/%u", &ind1, &ind2, &ind3, &ind4, &ind5, &ind6, &ind7, &ind8) == 8)
					{
						faces[c_faces].one.VertexIndex = ind1;
						faces[c_faces].one.TextureIndex = ind2;
						
						faces[c_faces].two.VertexIndex = ind3;
						faces[c_faces].two.TextureIndex = ind4;

						faces[c_faces].three.VertexIndex = ind5;
						faces[c_faces].three.TextureIndex = ind6;

						faces[c_faces].four.VertexIndex = ind7;
						faces[c_faces].four.TextureIndex = ind8;

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
	
	mesh->vertexCount = totalPoints;
	mesh->points = (MeshVertex*)OPalloc(sizeof(MeshVertex) * totalPoints);
	mesh->indices = (unsigned int*)OPalloc(sizeof(unsigned int) * totalIndices);
	int currPoint = 0;
	int currIndex = 0;

	for(i = 0; i < count; i++)
	{
		////Face Point One
		//MeshVertex p;
		//p.vertice = vertexes[faces[i].one.VertexIndex - 1];
		//p.tex = texes[faces[i].one.TextureIndex - 1];
		////p.normal = normals[faces[i].one.NormalIndex - 1];
		//mesh->points[currPoint++] = p;

		
		mesh->points[currPoint].vertice._x = vertexes[faces[i].one.VertexIndex - 1]._x;
		mesh->points[currPoint].vertice._y = vertexes[faces[i].one.VertexIndex - 1]._y;
		mesh->points[currPoint].vertice._z = vertexes[faces[i].one.VertexIndex - 1]._z;
		mesh->points[currPoint].tex._x = texes[faces[i].one.TextureIndex - 1]._x;
		mesh->points[currPoint].tex._y = texes[faces[i].one.TextureIndex - 1]._y;
		currPoint++;		
		
		mesh->points[currPoint].vertice._x = vertexes[faces[i].two.VertexIndex - 1]._x;
		mesh->points[currPoint].vertice._y = vertexes[faces[i].two.VertexIndex - 1]._y;
		mesh->points[currPoint].vertice._z = vertexes[faces[i].two.VertexIndex - 1]._z;
		mesh->points[currPoint].tex._x = texes[faces[i].two.TextureIndex - 1]._x;
		mesh->points[currPoint].tex._y = texes[faces[i].two.TextureIndex - 1]._y;
		currPoint++;		
		
		mesh->points[currPoint].vertice._x = vertexes[faces[i].three.VertexIndex - 1]._x;
		mesh->points[currPoint].vertice._y = vertexes[faces[i].three.VertexIndex - 1]._y;
		mesh->points[currPoint].vertice._z = vertexes[faces[i].three.VertexIndex - 1]._z;
		mesh->points[currPoint].tex._x = texes[faces[i].three.TextureIndex - 1]._x;
		mesh->points[currPoint].tex._y = texes[faces[i].three.TextureIndex - 1]._y;
		currPoint++;

		////Face Point Two
		//MeshVertex p2;
		//p2.vertice = vertexes[faces[i].two.VertexIndex - 1];
		//p2.tex = texes[faces[i].two.TextureIndex - 1];
		////p2.normal = normals[faces[i].two.NormalIndex - 1];
		//mesh->points[currPoint++] = p2;

		////Face Point Three
		//MeshVertex p3;
		//p3.vertice = vertexes[faces[i].three.VertexIndex - 1];
		//p3.tex = texes[faces[i].three.TextureIndex - 1];
		////p3.normal = normals[faces[i].three.NormalIndex - 1];
		//mesh->points[currPoint++] = p3;

		//Face Point Four
		//Only used if it's a quad, and not a triangle
		if(!faces[i].tri)
		{		
			mesh->points[currPoint].vertice._x = vertexes[faces[i].four.VertexIndex - 1]._x;
			mesh->points[currPoint].vertice._y = vertexes[faces[i].four.VertexIndex - 1]._y;
			mesh->points[currPoint].vertice._z = vertexes[faces[i].four.VertexIndex - 1]._z;
			mesh->points[currPoint].tex._x = texes[faces[i].four.TextureIndex - 1]._x;
			mesh->points[currPoint].tex._y = texes[faces[i].four.TextureIndex - 1]._y;
			currPoint++;

			//MeshVertex p4;
			//p4.vertice = vertexes[faces[i].four.VertexIndex - 1];
			//p4.tex = texes[faces[i].four.TextureIndex - 1];
			////p4.normal = normals[faces[i].four.NormalIndex - 1];
			//mesh->points[currPoint++] = p4;
		}

		//Push indices into the indices list. .obj file
		//creates triangles by (p1,p2,p3) for triangles
		//and (p1,p2,p3) (p1,p3,p4) for quads.
		mesh->indices[currIndex++] = place;
		mesh->indices[currIndex++] = place + 1;
		mesh->indices[currIndex++] = place + 2;
		if(!faces[i].tri)
		{
			mesh->indices[currIndex++] = place;
			mesh->indices[currIndex++] = place + 2;
			mesh->indices[currIndex++] = place + 3;
			place++;
		}
		place += 3;
	}

	delete[] vertexes;
	delete[] texes;
	delete[] normals;
	OPfree(buffer);
	
	OPLog("Indices");
	OPLog_i32(currIndex);
	OPLog_i32(totalIndices);
	OPLog_i32(count);
	OPLog_i32(totalPoints);
	//Primitive Count
	mesh->vertexCount = totalPoints;
	mesh->primitiveCount = totalIndices / 3;
	mesh->indicesCount = totalIndices;

	return mesh;
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