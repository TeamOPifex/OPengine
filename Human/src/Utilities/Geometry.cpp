#include "./Human/include/Utilities/Geometry.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Data/include/OPlist.h"

OPmesh* OPgeoCreateSphere(f32 radius, ui16 widthSegments, ui16 heightSegments) {
	f32 phiStart = 0;
	f32 phiLength = OPpi * 2;
	f32 thetaStart = 0;
	f32 thetaLength = OPpi * 2;

	ui16 x = 0, y = 0;
	ui16 elementCount = heightSegments * widthSegments;
	OPlist* vertices = OPlistCreate(64, sizeof(OPvec3));
	OPlist* normals = OPlistCreate(64, sizeof(OPvec3));
	OPlist* verticeIndices = OPlistCreate(64, sizeof(ui16));


	for (y = 0; y <= heightSegments; y++) {

		//var uvsRow = [];

		for (x = 0; x <= widthSegments; x++) {

			f32 u = x / (f32)widthSegments;
			f32 v = y / (f32)heightSegments;

			f32 vx = -radius * OPcos(phiStart + u * phiLength) * OPsin(thetaStart + v * thetaLength);
			f32 vy = radius * OPcos(thetaStart + v * thetaLength);
			f32 vz = radius * OPsin(phiStart + u * phiLength) * OPsin(thetaStart + v * thetaLength);

			OPvec3 vert = OPvec3Create(vx, vy, vz);
			OPlistPush(vertices, (ui8*)&vert);

			OPvec3 normal = OPvec3Norm(OPvec3Create(vx, vy, vz));

			OPlistPush(normals, (ui8*)&normal);

			ui16 pos = (ui16)OPlistSize(vertices) - 1;
			OPlistPush(verticeIndices, (ui8*)&pos);

			//uvsRow.push(new THREE.Vector2(u, 1 - v));

		}

		//uvs.push(uvsRow);

	}

	OPlist* indices = OPlistCreate(64, sizeof(ui16));

	for (y = 0; y < heightSegments; y++) {

		for (x = 0; x < widthSegments; x++) {

			ui16* v1 = (ui16*)OPlistGet(verticeIndices, y * heightSegments + x + 1);
			ui16* v2 = (ui16*)OPlistGet(verticeIndices, y * heightSegments + x);
			ui16* v3 = (ui16*)OPlistGet(verticeIndices, (y + 1) * heightSegments + x);
			ui16* v4 = (ui16*)OPlistGet(verticeIndices, (y + 1) * heightSegments + x + 1);


			//OPvec3 n1 = OPvec3clone((OPvec3*)OPlistGet(vertices, *v1));
			//OPvec3 n2 = OPvec3clone((OPvec3*)OPlistGet(vertices, *v2));
			//OPvec3 n3 = OPvec3clone((OPvec3*)OPlistGet(vertices, *v3));
			//OPvec3 n4 = OPvec3clone((OPvec3*)OPlistGet(vertices, *v4));

			//OPvec3norm(&n1, &n1);
			//OPvec3norm(&n2, &n2);
			//OPvec3norm(&n3, &n3);
			//OPvec3norm(&n4, &n4);

	//		var uv1 = uvs[y][x + 1].clone();
	//		var uv2 = uvs[y][x].clone();
	//		var uv3 = uvs[y + 1][x].clone();
	//		var uv4 = uvs[y + 1][x + 1].clone();

			if (OPabs(((OPvec3*)OPlistGet(vertices, *v1))->y) == radius) {

				OPlistPush(indices, (ui8*)v1);
				OPlistPush(indices, (ui8*)v3);
				OPlistPush(indices, (ui8*)v4);

				//uv1.x = (uv1.x + uv2.x) / 2;
				//this.faces.push(new THREE.Face3(v1, v3, v4, [n1, n3, n4]));
				//this.faceVertexUvs[0].push([uv1, uv3, uv4]);

			}
			else if (OPabs(((OPvec3*)OPlistGet(vertices, *v3))->y) == radius) {

				OPlistPush(indices, (ui8*)v1);
				OPlistPush(indices, (ui8*)v2);
				OPlistPush(indices, (ui8*)v3);
			}
			else {

				OPlistPush(indices, (ui8*)v1);
				OPlistPush(indices, (ui8*)v2);
				OPlistPush(indices, (ui8*)v4);


				OPlistPush(indices, (ui8*)v2);
				OPlistPush(indices, (ui8*)v3);
				OPlistPush(indices, (ui8*)v4);

			}

		}

	}

	OPuint count = OPlistSize(vertices);
	OPuint indCount = OPlistSize(indices);

	OPvec3* verts = (OPvec3*)OPalloc(sizeof(OPvec3) * count);
	OPvec3* norms = (OPvec3*)OPalloc(sizeof(OPvec3) * count);
	ui16* inds = (ui16*)OPalloc(sizeof(ui16)* indCount);

	for (OPuint i = 0; i < count; i++) {
		OPvec3* vec = (OPvec3*)OPlistGet(vertices, i);
		OPvec3* norm = (OPvec3*)OPlistGet(normals, i);
		OPmemcpy(&verts[i], vec, sizeof(OPvec3));
		OPmemcpy(&norms[i], norm, sizeof(OPvec3));
	}
	for (OPuint i = 0; i < indCount; i++) {
		ui16* ind = (ui16*)OPlistGet(indices, i);
		OPmemcpy(&inds[i], ind, sizeof(ui16));
	}

	OPvertices* verticesStructure = OPverticesCreate(count, Position | Normal);
	OPverticesWriteVec3(verticesStructure, verts, Position);
	OPverticesWriteVec3(verticesStructure, norms, Normal);

	OPmesh* mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
	*mesh = OPmeshCreate();
	mesh->Bind();

	OPmeshBuild(
		verticesStructure->size * sizeof(f32), 
		OPindexSize::SHORT,
		count, 
		indCount, 
		verticesStructure->data, 
		inds);
	
	return mesh;
}