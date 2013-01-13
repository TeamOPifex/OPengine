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

OBJMesh* LoadOBJ(FILE* file, int start, int length)
{
	//ObjectMesh to fill
	OBJMesh* mesh = new OBJMesh();

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
				total_faces++;
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
	Vector2* texes = new Vector2[total_texs];
	Vector3* normals = new Vector3[total_norms];
	Face* faces = new Face[total_faces];
	
	offset = 0;
	fseek(file, start, SEEK_SET);
	
	int x, y, z, w;
	int x2, y2, z2, w2;

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
					texes[c_texs] = Vector2(x, y);
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
					if (sscanf(buffer, "f %u/%u %u/%u %u/%u", &x, &y, &z, &x2, &y2, &z2) == 6)
					{
						faces[c_faces].one.VertexIndex = x;
						faces[c_faces].one.TextureIndex = y;
						
						faces[c_faces].two.VertexIndex = z;
						faces[c_faces].two.TextureIndex = x2;

						faces[c_faces].three.VertexIndex = y2;
						faces[c_faces].three.TextureIndex = z2;

						faces[c_faces].tri = true;
						c_faces++;
					}
				}
				else if (cnt == 4)
				{
					if (sscanf(buffer, "f %u/%u %u/%u %u/%u %u/%u", &x, &y, &z, &w, &x2, &y2, &z2, &w2) == 8)
					{
						faces[c_faces].one.VertexIndex = x;
						faces[c_faces].one.TextureIndex = y;
						
						faces[c_faces].two.VertexIndex = z;
						faces[c_faces].two.TextureIndex = w;
						
						faces[c_faces].three.VertexIndex = x2;
						faces[c_faces].three.TextureIndex = y2;

						faces[c_faces].four.VertexIndex = z2;
						faces[c_faces].four.TextureIndex = w2;

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
	mesh->points = (OBJMeshPoint*)OPalloc(sizeof(OBJMeshPoint) * totalPoints);
	mesh->indices = (int*)OPalloc(sizeof(int) * totalIndices);
	int currPoint = 0;
	int currIndex = 0;

	for(i = 0; i < count; i++)
	{
		//Face Point One
		OBJMeshPoint p;
		p.vertice = vertexes[faces[i].one.VertexIndex - 1];
		p.tex = texes[faces[i].one.TextureIndex - 1];
		p.normal = normals[faces[i].one.NormalIndex - 1];
		mesh->points[currPoint++] = p;


		//Face Point Two
		OBJMeshPoint p2;
		p2.vertice = vertexes[faces[i].two.VertexIndex - 1];
		p2.tex = texes[faces[i].two.TextureIndex - 1];
		p2.normal = normals[faces[i].two.NormalIndex - 1];
		mesh->points[currPoint++] = p2;

		//Face Point Three
		OBJMeshPoint p3;
		p3.vertice = vertexes[faces[i].three.VertexIndex - 1];
		p3.tex = texes[faces[i].three.TextureIndex - 1];
		p3.normal = normals[faces[i].three.NormalIndex - 1];
		mesh->points[currPoint++] = p3;

		//Face Point Four
		//Only used if it's a quad, and not a triangle
		if(!faces[i].tri)
		{
			OBJMeshPoint p4;
			p4.vertice = vertexes[faces[i].four.VertexIndex - 1];
			p4.tex = texes[faces[i].four.TextureIndex - 1];
			p4.normal = normals[faces[i].four.NormalIndex - 1];
			mesh->points[currPoint++] = p4;
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

	//Primitive Count
	mesh->primitiveCount = totalIndices / 3;
	mesh->position = Vector3(0,0,0);

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
//
//void FillFacePoint(FacePoint* point, string line)
//{
//	int offset = line.find_first_of('/',0);
//	int offset2 = line.find_first_of('/',offset + 1);
//	string num1 = line.substr(0, offset);
//	string num2 = line.substr(offset + 1, offset2 - offset - 1);
//	string num3 = line.substr(offset2 + 1, line.length() - offset2);
//	point->VertexIndex = atoi(num1.c_str());
//	point->TextureIndex = atoi(num2.c_str());
//	point->NormalIndex = atoi(num3.c_str());
//}

//std::wstring s2ws(const std::string& s)
//{
//    int len;
//    int slength = (int)s.length() + 1;
//    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
//    wchar_t* buf = new wchar_t[len];
//    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
//    std::wstring r(buf);
//    delete[] buf;
//    return r;
//}