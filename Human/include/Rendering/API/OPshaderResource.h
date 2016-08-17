#pragma once

struct OPshaderResource;
typedef struct OPshaderResource OPshaderResource;

struct OPshaderResource {
	void(*Bind)(void* ptr) = 0;
	void(*Unbind)(void* ptr) = 0;
};