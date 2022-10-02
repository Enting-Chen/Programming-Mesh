#include "Mesh.h"
#ifdef CodeExecutable

Mesh::Mesh() {
	element_list_index = 0;
	map_list_index = 0;
	dat_list_index = 0;
	element_list_size = 10;
	map_list_size = 10;
	dat_list_size = 10;
	element_list = (Elements*)malloc(sizeof(Elements) * element_list_size);
	map_list = (Map*)malloc(sizeof(Map) * map_list_size);
	dat_list = (Data*)malloc(sizeof(Data) * dat_list_size);
	memset(element_list, 0, sizeof(Elements) * map_list_size);
	memset(map_list, 0, sizeof(Map) * element_list_size);
	memset(dat_list, 0, sizeof(Data) * dat_list_size);
}

Mesh::Mesh(char* fname) {
	element_list_index = 0;
	map_list_index = 0;
	dat_list_index = 0;
	element_list_size = 10;
	map_list_size = 10;
	dat_list_size = 10;
	    element_list = (Elements*)malloc( sizeof( Elements ) * element_list_size );
    map_list = (Map*)malloc( sizeof( Map ) * map_list_size );
    dat_list = (Data*)malloc( sizeof( Data ) * dat_list_size );
    memset( element_list, 0, sizeof( Map ) * map_list_size );
    memset( map_list, 0, sizeof( Elements ) * element_list_size );
    memset( dat_list, 0, sizeof( Data ) * dat_list_size );
	ReadMeshFromEMDAscii(fname);
}

Mesh::~Mesh() {
	//to do sth
	/*
	for (int i = 0; i < element_list_index; i++) {
		free(element_list[i]);
	}
	free(element_list);
	for (int i = 0; i < map_list_index; i++) {
		free(map_list[i]->map);
		//free(map_list[i]);
	}
	free(map_list);
	for (int i = 0; i < dat_list_index; i++) {
		free(dat_list[i]->data);
		free(dat_list[i]);
	}
	free(dat_list);*/
}

////////////////////////////////////////////////
void Mesh::init() {
	//to do sth
	if (element_list_index) {
		for (int i = 0; i < element_list_index; i++) {
			free(element_list[i]);
		}
		free(element_list);
	}
	if (map_list_index) {
		for (int i = 0; i < map_list_index; i++) {
			free(map_list[i]->map);
			//free(map_list[i]);
		}
		free(map_list);
	}
	if (dat_list_index) {
		for (int i = 0; i < dat_list_index; i++) {
			free(dat_list[i]->data);
			free(dat_list[i]);
		}
		free(dat_list);
	}
	element_list_index = 0;
	map_list_index = 0;
	dat_list_index = 0;
	element_list_size = 10;
	map_list_size = 10;
	dat_list_size = 10;
	element_list = (Elements*)malloc(sizeof(Elements) * element_list_size);
	map_list = (Map*)malloc(sizeof(Map) * map_list_size);
	dat_list = (Data*)malloc(sizeof(Data) * dat_list_size);
	memset(element_list, 0, sizeof(Elements) * map_list_size);
	memset(map_list, 0, sizeof(Map) * element_list_size);
	memset(dat_list, 0, sizeof(Data) * dat_list_size);
}
Elements Mesh::makeElements(int ele_size, char const* ele_name) {
	element_list[element_list_index] = (Elements)malloc(sizeof(elements));
	strcpy((char*)(element_list[element_list_index]->name), ele_name);
	element_list[element_list_index]->size = ele_size;
	element_list[element_list_index]->index = element_list_index;
	element_list_index++;
	return element_list[element_list_index-1];
}
Map Mesh::makeMap(Elements map_from, Elements map_to, int map_dim, int* map_map, char const* map_name) {
	int arr_cnt = map_from->size;
	map_list[map_list_index] = (Map)malloc(sizeof(map));
	map_list[map_list_index]->index = map_list_index;
	map_list[map_list_index]->from = map_from;
	map_list[map_list_index]->to = map_to;
	map_list[map_list_index]->dim = map_dim;
	//map_list[map_list_index]->map = map_map;
	map_list[map_list_index]->map = (int*)malloc(sizeof(int) * map_dim * arr_cnt);
	for (int i = 0; i < arr_cnt * map_dim; i++)
		map_list[map_list_index]->map[i] = map_map[i];
	strcpy((char*)(map_list[map_list_index]->name), map_name);
	map_list_index++;
	return map_list[map_list_index - 1];

}
Data Mesh::makeData(Elements data_set, int data_dim, char const* data_type, char* data_data, char const* data_name) {
	Data data = (Data)malloc(sizeof(dat));
	data->index = dat_list_index;
	data->set = data_set;
	data->dim = data_dim;
	//data->size = 8;   //xxxxxxxxxxx    ///////////////////////////
	//data->data = data_data;
	if (!strcmp(data_type, "int")) {
		data->size = 4;
		int* t = (int*)malloc(sizeof(int) * data->set->size * data->dim);
		for (int j = 0; j < data->set->size * data->dim; j++)
			t[j] = ((int*)data_data)[j];
		data->data = (char*)t;
	}
	if (!strcmp(data_type, "double")) {
		data->size = 8;
		double* t = (double*)malloc(sizeof(double) * data->set->size * data->dim);
		for (int j = 0; j < data->set->size * data->dim; j++)
			t[j] = ((double*)data_data)[j];
		data->data = (char*)t;
	}
	strcpy((char*)(data->name), data_name);
	strcpy((char*)(data->type), data_type);
	dat_list[dat_list_index++] = data;
	return data;
}

bool Mesh::ReadMeshFromEMDAscii(const char* fileName) {     /////////////////////////////
	FILE* fp;
	if ((fp = fopen(fileName, "rb")) == NULL) {
		printf("can't open file\n");
		return 0;
	}
	
	if (element_list_index) {
		for (int i = 0; i < element_list_index; i++) {
			free(element_list[i]);
		}
		free(element_list);
	}
	if (map_list_index) {
		for (int i = 0; i < map_list_index; i++) {
			free(map_list[i]->map);
			//free(map_list[i]);
		}
		free(map_list);
	}
	if (dat_list_index) {
		for (int i = 0; i < dat_list_index; i++) {
			free(dat_list[i]->data);
			free(dat_list[i]);
		}
		free(dat_list);
	}
	readHeader(fp);
	element_list = (Elements*)malloc(sizeof(Elements) * element_list_size);
	map_list = (Map*)malloc(sizeof(Map) * map_list_size);
	dat_list = (Data*)malloc(sizeof(Data) * dat_list_size);
	readElements(fp);
	readMaps(fp);
	readData(fp);

	fclose(fp);
	return true;
}

bool Mesh::WriteMeshToEMDAscii(const char* fileName) {    //////////////////////////
	printf("writing in grid \n");
	FILE* fp;
	if ((fp = fopen(fileName, "wb")) == NULL) {
		printf("can't open file\n");
		return 0;
	}

	writeHeader(fp);
	writeElements(fp);
	writeMaps(fp);
	writeData(fp);

	fclose(fp);
	return true;
}

bool Mesh::writeHeaderbin(FILE * fp) {   //////////////////////
	int header[6] = { element_list_size, map_list_size, dat_list_size,
			element_list_index, map_list_index, dat_list_index };
	int count = fwrite(header, sizeof(int), 6, fp);
	if (count != 6)
		return 0;
	return 1;
}

bool Mesh::writeElementsbin(FILE * fp) {   ////////////////////
	for (int i = 0; i < element_list_index; i++) {
		int count = fwrite(element_list[i], sizeof(elements), 1, fp);
		if (count != 1)
			return 0;
	}
	return 1;
}

bool Mesh::writeMapsbin(FILE * fp) {    /////////////////////////
	for (int i = 0; i < map_list_index; i++) {
		int temp[5] = { map_list[i]->index, map_list[i]->from->index,
			map_list[i]->to->index, map_list[i]->dim,strlen(map_list[i]->name) };
		int count = fwrite(temp, sizeof(int), 5, fp);
		if (count != 5)
			return 0;
		count = fwrite(map_list[i]->name, sizeof(char), temp[4], fp);
		if (count != temp[4])
			return 0;
		count = fwrite(map_list[i]->map, sizeof(int), map_list[i]->from->size * map_list[i]->dim, fp);
		if (count != map_list[i]->from->size * map_list[i]->dim)
			return 0;
	}
	return 1;
}
bool Mesh::writeDatabin(FILE * fp) {    ////////////////
	for (int i = 0; i < dat_list_index; i++) {
		int temp[5] = { dat_list[i]->index, dat_list[i]->set->index,
			dat_list[i]->dim, dat_list[i]->size,strlen(dat_list[i]->name) };
		int count = fwrite(temp, sizeof(int), 5, fp);
		if (count != 5)
			return 0;
		count = fwrite(dat_list[i]->name, sizeof(char), temp[4], fp);
		if (count != temp[4])
			return 0;
		if (temp[3] == 8) {
			double* temp = (double*)dat_list[i]->data;
			int msize = dat_list[i]->dim * dat_list[i]->set->size;
			int count = fwrite(temp, sizeof(double), msize, fp);
			if (count != msize)
				return 0;
		}
		else if (temp[3] == 4) {
			int count = fwrite(dat_list[i]->data, sizeof(int), dat_list[i]->dim * dat_list[i]->set->size, fp);
			if (count != dat_list[i]->dim * dat_list[i]->set->size)
				return 0;
		}
	}
	return 1;
}

bool Mesh::readHeaderbin(FILE * fp) {   ///////////////////
	int header[6];
	int count = fread(header, sizeof(int), 6, fp);
	if (count != 6)
		return 0;
	element_list_size = header[0];
	map_list_size = header[1];
	dat_list_size = header[2];
	element_list_index = header[3];
	map_list_index = header[4];
	dat_list_index = header[5];
	return 1;
}

bool Mesh::readElementsbin(FILE * fp) {    ////////////////////
	for (int i = 0; i < element_list_index; i++) {
		element_list[i] = (Elements)malloc(sizeof(elements));
		int count = fread(element_list[i], sizeof(elements), 1, fp);
		if (count != 1)
			return 0;
	}
	return 1;
}

bool Mesh::readMapsbin(FILE * fp) {    ///////////////////////////
	for (int i = 0; i < map_list_index; i++) {
		int temp[5];
		int count = fread(temp, sizeof(int), 5, fp);
		if (count != 5)
			return 0;
		map_list[i] = (Map)malloc(sizeof(map));
		map_list[i]->index = temp[0];
		map_list[i]->from = element_list[temp[1]];
		map_list[i]->to = element_list[temp[2]];
		map_list[i]->dim = temp[3];
		char t[100] = { 0 };
		count = fread(t, sizeof(char), temp[4], fp);
		if (count != temp[4])
			return 0;
		strcpy((char*)(map_list[i]->name), t);
		map_list[i]->map = (int*)malloc(map_list[i]->dim * map_list[i]->from->size * sizeof(int));
		count = fread(map_list[i]->map, sizeof(int), map_list[i]->dim * map_list[i]->from->size, fp);
		if (count != map_list[i]->dim * map_list[i]->from->size)
			return 0;
	}
	return 1;
}
bool Mesh::readDatabin(FILE * fp) {     /////////////////////
	for (int i = 0; i < dat_list_index; i++) {
		int temp[5];
		dat_list[i] = (Data)malloc(sizeof(dat));
		int count = fread(temp, sizeof(int), 5, fp);
		if (count != 5)
			return 0;
		char nam[100] = { 0 };
		count = fread(nam, sizeof(char), temp[4], fp);
		if (temp[4] != count)
			return 0;
		strcpy((char*)(dat_list[i]->name), nam);
		dat_list[i]->index = temp[0];
		dat_list[i]->set = element_list[temp[1]];
		dat_list[i]->dim = temp[2];
		dat_list[i]->size = temp[3];
		if (temp[3] == 4) {
			int* t = (int*)malloc(sizeof(int) * dat_list[i]->dim * dat_list[i]->set->size);
			count = fread(t, sizeof(int), dat_list[i]->dim * dat_list[i]->set->size, fp);
			if (count != dat_list[i]->dim * dat_list[i]->set->size)
				return 0;
			dat_list[i]->data = (char*)t;
		}
		else if (temp[3] == 8) {
			int msize = dat_list[i]->dim * dat_list[i]->set->size;
			double* t = (double*)malloc(sizeof(double) * (msize + 5));
			int count = fread(t, sizeof(double), msize, fp);
			if (count != msize)
				return 0;
			dat_list[i]->data = (char*)t;
		}
	}
	return 1;
}

/////////////////////////////////////// vtk /////////////////////////
bool Mesh::readvtk(const char* fileName) {
/*	printf("reading in grid \n");

	FILE* fp;
	if ((fp = fopen(fileName, "r")) == NULL) {
		printf("can't open file\n");
		return 0;
	}
	//put your code here
	readvtkHeader(fp);// 读取头
	char str[100];
	while (fscanf(fp, "%s", str) != EOF) {
		if (strcmp(str, "POINTS") == 0) {
			readvtkPoints(fp);
		}
		else if (strcmp(str, "CELLS") == 0) {
			readvtkCells(fp);
		}
		else if (strcmp(str, "CELL_TYPES") == 0) {
			readvtkCellTypes(fp);
		}
		else if (strcmp(str, "CELL_DATA") == 0) {
			readvtkCellData(fp);
		}
		else if (strcmp(str, "POINT_DATA") == 0) {
			readvtkPointData(fp);
		}
	}

	fclose(fp); */
	return true;
}

bool Mesh::savevtk(const char* fileName) {
	printf("writing in grid \n");
	FILE* fp;
	if ((fp = fopen(fileName, "w")) == NULL) {
		printf("can't open file\n");
		return 0;
	}

	//put your code here
	writevtkHeader(fp);
	writevtkPoints(fp);
	writevtkCells(fp);
	writevtkCellType(fp);
	writevtkCellData(fp);


	fclose(fp);
	return true;
}

bool Mesh::readvtkHeader(FILE * fp, char* meshDescription) {
	char str[100];
	fscanf(fp, "%[^\n]%*c", str);
	fscanf(fp, "%[^\n]%*c", meshDescription); //
	fscanf(fp, "%[^\n]%*c", str); // ASCII
	if (strcmp(str, "ASCII") != 0) {
		exit(-1);
	}
	fscanf(fp, "%[^\n]%*c", str); //数据信息

	return 0;
}

bool Mesh::readvtkPoints(FILE * fp, int& npoint, double*& xyz) {
	char str[100];
	fscanf(fp, "%s", str);  //POINTS
	fscanf(fp, "%d %s\n", &npoint, str);
	if (strcmp(str, "double") == 0) {
		xyz = (double*)malloc(3 * npoint * sizeof(double));
	}
	for (int n = 0; n < npoint; n++) {
		fscanf(fp, "%lf %lf %lf\n", &(xyz[3 * n]), &(xyz[3 * n + 1]), &(xyz[3 * n + 2]));
	}
	return 0;
}

bool Mesh::readvtkCells(FILE * fp, int& ncell, int& cellsize, int*& cells) {
	char str[100];
	fscanf(fp, "%s", str);  //CELLS
	fscanf(fp, "%d %d\n", &ncell, &cellsize);
	cells = (int*)malloc(cellsize * sizeof(int));
	int count = 0, tt;
	while (count < cellsize) {
		fscanf(fp, "%d\n", &tt);
		int vex = tt;
		//count++;
		while (vex) {
			vex--;
			fscanf(fp, "%d\n", &cells[count++]);
		}
	}
	return 0;
}

bool Mesh::readvtkCellTypes(FILE * fp, int ncell, int*& celltypes) {
	char str[100];
	fscanf(fp, "%s", str);  //CELL_TYPES
	fscanf(fp, "%d\n", &ncell);
	celltypes = (int*)malloc(ncell * sizeof(int));
	for (int n = 0; n < ncell; n++) {
		fscanf(fp, "%d \n", &celltypes[n]);
	}
	return 0;
}

bool Mesh::readvtkPointData(FILE * fp, int npoint, int& type, int& dim, char*& pointdata) {
	char str[100];
	fscanf(fp, "%s", str);  //POINT_DATA
	fscanf(fp, "%d\n", &npoint);
	fscanf(fp, "%s", str);//SCALARS
	fscanf(fp, "%s", str);//CellEntityIds
	fscanf(fp, "%s", str);//double
	if (strcmp(str, "double") == 0) {
		type = 8;
	}
	else if (strcmp(str, "int") == 0) {
		type = 4;
	}
	else type = 1;
	fscanf(fp, "%d\n", &dim);
	pointdata = (char*)malloc(npoint * dim * type * sizeof(char));   //npoint
	//fscanf(fp, "%[^\n]%*c", str);
	//int npComp = str[strlen(str) - 1] - 48;
	fscanf(fp, "%[^\n]%*c", str);
	if (type == 8) {
		for (int n = 0; n < npoint; n++) {
			for (int d = 0; d < dim; d++)
				fscanf(fp, "%lf ", &((double*)pointdata)[n * dim + d]);
			fscanf(fp, "\n");
		}
	}
	else if (type == 4) {
		for (int n = 0; n < npoint; n++) {
			for (int d = 0; d < dim; d++)
				fscanf(fp, "%d ", &((int*)pointdata)[n * dim + d]);
			fscanf(fp, "\n");
		}
	}
	else {
		for (int n = 0; n < npoint; n++) {
			for (int d = 0; d < dim; d++)
				fscanf(fp, "%c ", &pointdata[n * dim + d]);
			fscanf(fp, "\n");
		}
	}
	return 0;
}

bool Mesh::readvtkCellData(FILE * fp, int ncell, int& type, int& dim, char*& celldata) {
	char str[100];
	fscanf(fp, "%s", str);  //CELL_DATA
	fscanf(fp, "%d\n", &ncell);
	fscanf(fp, "%s", str);//SCALARS
	fscanf(fp, "%s", str);//CellEntityIds
	fscanf(fp, "%s", str);//double
	if (strcmp(str, "double") == 0) {
		type = 8;
	}
	else if (strcmp(str, "int") == 0) {
		type = 4;
	}
	else type = 1;
	fscanf(fp, "%d\n", &dim);
	celldata = (char*)malloc(ncell * type * dim * sizeof(char));
	/*fscanf(fp, "%[^\n]%*c", str);
	int ncComp = str[strlen(str) - 1] - 48;*/
	fscanf(fp, "%[^\n]%*c", str);
	if (type == 8) {
		for (int n = 0; n < ncell; n++) {
			for (int d = 0; d < dim; d++)
				fscanf(fp, "%lf ", &((double*)celldata)[n * dim + d]);
			fscanf(fp, "\n");
		}
	}
	else if (type == 4) {
		for (int n = 0; n < ncell; n++) {
			for (int d = 0; d < dim; d++)
				fscanf(fp, "%d ", &((int*)celldata)[n * dim + d]);
			fscanf(fp, "\n");
		}
	}
	else {
		for (int n = 0; n < ncell; n++) {
			for (int d = 0; d < dim; d++)
				fscanf(fp, "%c ", &celldata[n * dim + d]);
			fscanf(fp, "\n");
		}
	}
	return 0;
}

bool Mesh::writevtkHeader(FILE* fp) {
	fprintf(fp, "# vtk DataFile Version 2.0\n");
	fprintf(fp, "vtkDescription\n");
	fprintf(fp, "ASCII\n");
	fprintf(fp, "DATASET UNSTRUCTURED_GRID\n");
	return 1;
}

bool Mesh::writevtkPoints(FILE* fp) {
	int npoint = element_list[0]->size;
	double* xyz = (double*)dat_list[1]->data;
	fprintf(fp, "POINTS %d double\n", npoint);
	for (int n = 0; n < npoint; n++) {
		fprintf(fp, "%lf %lf %lf\n", xyz[3 * n], xyz[3 * n + 1], xyz[3 * n + 2]);
	}
	return 1;
}

bool Mesh::writevtkCells(FILE * fp) {/*
	fprintf(fp, "\nCELLS %d %d\n", ncell, cellsize);
	for (int n = 0; n < 4; n++) {
		fprintf(fp, "%d %d \n", cells[2 * n], cells[2 * n + 1]);
	}
	for (int n = 4; n < 12; n++) {
		fprintf(fp, "%d %d %d \n", cells[3 * n - 4], cells[3 * n - 3], cells[3 * n - 2]);
	}
	for (int n = 12; n < ncell; n++) {
		fprintf(fp, "%d %d %d %d \n", cells[4 * n - 16], cells[4 * n - 15], cells[4 * n - 14], cells[4 * n - 13]);
	}*/
	return 1;
}
bool Mesh::writevtkCellType(FILE * fp) {/*
	fprintf(fp, "\nCELL_TYPES %d \n", ncell);
	for (int n = 0; n < ncell; n++) {
		fprintf(fp, "%d \n", celltypes[n]);
	}*/
	return 1;
}
bool Mesh::writevtkCellData(FILE * fp) {/*
	fprintf(fp, "\nCELL_DATA %d \n", ncell);
	fprintf(fp, "SCALARS CellEntityIds int %d\n", ncComp);
	fprintf(fp, "LOOKUP_TABLE default\n");
	for (int n = 0; n < ncell; n++) {
		fprintf(fp, "%d \n", celldata[n]);
	}*/
	return 1;
}

bool Mesh::writevtkPointData(FILE * fp) {
	return 1;
}

//////////////////////////////////////////////////raw file reading
bool readraw(const char* fileName) {
	FILE* fp;
	if ((fp = fopen(fileName, "r")) == NULL) {
		printf("can't open file\n");
		return 0;
	}
	int nnode, ncell, nedge, nbedge;
	//readrawHeader(fp);
	int *cell = (int*)malloc(4 * ncell * sizeof(int));
	int* edge = (int*)malloc(2 * nedge * sizeof(int));
	int* ecell = (int*)malloc(2 * nedge * sizeof(int));
	int* bedge = (int*)malloc(2 * nbedge * sizeof(int));
	int* becell = (int*)malloc(nbedge * sizeof(int));
	int* bound = (int*)malloc(nbedge * sizeof(int));
	double *x = (double*)malloc(3 * nnode * sizeof(double));

	//readrawNode(fp);
	//readrawCell(fp);
	//readrawEdge(fp);
	//readrawBEdge(fp);

	fclose(fp);
	return true;
}


bool Mesh::writeHeader(FILE * fp) {   //////////////////////
	// using fprintf
	fprintf(fp, "%d %d %d %d %d %d\n", element_list_size, map_list_size, dat_list_size,
		element_list_index, map_list_index, dat_list_index);
	return 1;
}

bool Mesh::writeElements(FILE * fp) {   ////////////////////
	// using fprintf
	for ( int i = 0; i < element_list_index; i++ ){
		fprintf(fp, "%d %d %s\n", element_list[i]->index, element_list[i]->size, element_list[i]->name);
	} 
	return 1;
}

bool Mesh::writeMaps(FILE * fp) {    /////////////////////////
	// using fprintf
	for (int i = 0; i < map_list_index; i++) {
		fprintf(fp, "%d %d %d %d %s\n", map_list[i]->index, map_list[i]->from->index,
			map_list[i]->to->index, map_list[i]->dim, map_list[i]->name);
		for (int j = 0; j < map_list[i]->from->size; j++) {
			for (int k = 0; k < map_list[i]->dim; k++) {
				fprintf(fp, " %d", map_list[i]->map[j * map_list[i]->dim + k]);
			}
			fprintf(fp, "\n");
		}
		fprintf(fp, "\n");
	}
	return 1;
}

bool Mesh::writeData(FILE * fp) {    ////////////////
	// using fscanf
	for ( int i = 0; i < dat_list_index; i++ ){
		fprintf(fp, "%d %d %d %d %s\n", dat_list[i]->index, dat_list[i]->set->index,
			dat_list[i]->dim, dat_list[i]->size, dat_list[i]->name);
		if (dat_list[i]->size == 8 ){
			for ( int k = 0; k < dat_list[i]->set->size; k++ ){
				for ( int j = 0; j < dat_list[i]->dim; j++ ){
					fprintf(fp, " %f", dat_list[i]->data[k * dat_list[i]->dim + j]);
				}
				fprintf(fp, "\n");
			}
		}
		else if (dat_list[i]->size == 4 ){
			for ( int k = 0; k < dat_list[i]->set->size; k++ ){
				for ( int j = 0; j < dat_list[i]->dim; j++ ){
					fprintf(fp, " %d", dat_list[i]->data[k * dat_list[i]->dim + j]);
				}
				fprintf(fp, "\n");
			}
		}
		fprintf(fp, "\n");
	}
	return 1;
}

bool Mesh::readHeader(FILE * fp) {   ///////////////////
	int header[6];
	int count = fscanf(fp, "%d %d %d %d %d %d", &element_list_size, &map_list_size, &dat_list_size, 
	&element_list_index, &map_list_index, &dat_list_index);
		return 0;
	return 1;
}

bool Mesh::readElements(FILE * fp) {    ////////////////////
	// using fscanf 
	for ( int i = 0; i < element_list_index; i++ ){
		element_list[i] = (Elements)malloc(sizeof(elements));
		int count = fscanf(fp, "%d %d %s", &element_list[i]->index, &element_list[i]->size, element_list[i]->name);
		if (count != 3)
			return 0;
	}
	return 1;
}

bool Mesh::readMaps(FILE * fp) {    ///////////////////////////
	// using fscanf
	for ( int i = 0; i < map_list_index; i++ ){
		int temp[4];
		int count = fscanf(fp, "%d %d %d %d", &temp[0], &temp[1], &temp[2], &temp[3]);
		if (count != 4)
			return 0;
		map_list[i] = (Map)malloc(sizeof(map));
		map_list[i]->index = temp[0];
		map_list[i]->from = element_list[temp[1]];
		map_list[i]->to = element_list[temp[2]];
		map_list[i]->dim = temp[3];
		char t[100] = { 0 };
		count = fscanf(fp, "%s", t);
		if (count != 1)
			return 0;
		strcpy((char*)(map_list[i]->name), t);
		map_list[i]->map = (int*)malloc(map_list[i]->dim * map_list[i]->from->size * sizeof(int));
		for (int j = 0; j < map_list[i]->from->size; j++) {
			for (int k = 0; k < map_list[i]->dim; k++) {
				count = fscanf(fp, "%d", &map_list[i]->map[j * map_list[i]->dim + k]);
				if (count != 1)
					return 0;
			}
		}
	}
	return 1;
}

bool Mesh::readData(FILE * fp) {     /////////////////////
	// using fscanf
	for ( int i = 0; i < dat_list_index; i++ ){
		int temp[4];
		dat_list[i] = (Data)malloc(sizeof(dat));
		int count = fscanf(fp, "%d %d %d %d", &temp[0], &temp[1], &temp[2], &temp[3]);
		if (count != 4)
			return 0;
		char nam[100] = { 0 };
		count = fscanf(fp, "%s", nam);
		if (count != 1)
			return 0;
		strcpy((char*)(dat_list[i]->name), nam);
		dat_list[i]->index = temp[0];
		dat_list[i]->set = element_list[temp[1]];
		dat_list[i]->dim = temp[2];
		dat_list[i]->size = temp[3];
		int msize = dat_list[i]->dim * dat_list[i]->set->size;
		if (temp[3] == 4) {
			int* t = (int*)malloc(sizeof(int) * msize);
			for (int j = 0; j < msize; j++)
				fscanf(fp, "%d", &t[j]);
			dat_list[i]->data = (char*)t;
		}
		else if (temp[3] == 8) {
			double* t = (double*)malloc(sizeof(double) * msize);
			for (int j = 0; j < msize; j++)
				fscanf(fp, "%lf", &t[j]);
			dat_list[i]->data = (char*)t;
		}
	}
	return 1;
}



bool readrawHeader(FILE* fp, int* nnode, int* ncell, int* nedge, int* nbedge) {
	return fscanf(fp, "%d %d %d %d \n", nnode, ncell, nedge, nbedge) == 4;
}

bool readrawNode(FILE* fp, int nnode, double* x) {
	for (int n = 0; n < nnode; n++) {
		if (fscanf(fp, "%lf %lf \n", &x[3 * n], &x[3 * n + 1]) != 2) {
			printf("error reading from 1new_grid.dat\n"); exit(-1);
		}
		x[3 * n + 2] = 0;
	}
	return 1;
}

bool readrawCell(FILE* fp,int ncell, int* cell) {
	for (int n = 0; n < ncell; n++) {
		if (fscanf(fp, "%d %d %d %d \n", &cell[4 * n], &cell[4 * n + 1],
			&cell[4 * n + 2], &cell[4 * n + 3]) != 4) {
			printf("error reading from 2new_grid.dat\n"); exit(-1);
		}
	}
	return 1;
}

bool readrawEdge(FILE* fp, int nedge,int* edge, int* ecell) {
	for (int n = 0; n < nedge; n++) {
		if (fscanf(fp, "%d %d %d %d \n", &edge[2 * n], &edge[2 * n + 1],
			&ecell[2 * n], &ecell[2 * n + 1]) != 4) {
			printf("error reading from 3new_grid.dat\n"); exit(-1);
		}
	}
	return 1;
}

bool readrawBEdge(FILE* fp, int nbedge, int* bedge, int* becell, int* bound) {
	for (int n = 0; n < nbedge; n++) {
		if (fscanf(fp, "%d %d %d %d \n", &bedge[2 * n], &bedge[2 * n + 1],
			&becell[n], &bound[n]) != 4) {
			printf("error reading from new_grid.dat\n"); exit(-1);
		}
	}
	return 1;
}

bool readrawQ(FILE* fp, int ncell, double* q) {
	for (int n = 0; n < ncell; n++) {
		if (fscanf(fp, "%lf %lf %lf %lf \n", &q[4 * n], &q[4 * n + 1],
			&q[4 * n + 2], &q[4 * n + 3]) != 4) {
			printf("error reading from new_grid.dat\n"); exit(-1);
		}
	}
	return 1;
}

bool writerawNode(FILE* fp, int nnode, double* x) {
	for (int n = 0; n < nnode; n++) {
		fprintf(fp, "%lf %lf \n", x[3 * n], x[3 * n + 1]);
	}
	return 1;
}

bool writerawCell(FILE* fp, int ncell, int* cell) {
	for (int n = 0; n < ncell; n++) {
		fprintf(fp, "%d %d %d %d \n", cell[4 * n], cell[4 * n + 1],
			cell[4 * n + 2], cell[4 * n + 3]);
	}
	return 1;
}

bool writerawEdge(FILE* fp, int nedge, int* edge, int* ecell) {
	for (int n = 0; n < nedge; n++) {
		fprintf(fp, "%d %d %d %d \n", edge[2 * n], edge[2 * n + 1],
			ecell[2 * n], ecell[2 * n + 1]);
	}
	return 1;
}

bool writerawBEdge(FILE* fp, int nbedge, int* bedge, int* becell, int* bound) {
	for (int n = 0; n < nbedge; n++) {
		fprintf(fp, "%d %d %d %d \n", bedge[2 * n], bedge[2 * n + 1],
			becell[n], bound[n]);
	}
	return 1;
}

bool writerawHeader(FILE* fp, int nnode, int ncell, int nedge, int nbedge) {
	fprintf(fp, "%d %d %d %d \n", nnode, ncell, nedge, nbedge);
	return 1;
}

bool writerawQ(FILE* fp, int ncell, double* q) {
	for (int n = 0; n < ncell; n++) {
		fprintf(fp, "%lf %lf %lf %lf \n", q[4 * n], q[4 * n + 1],
			q[4 * n + 2], q[4 * n + 3]);
	}
	return 1;
}

/*
 * Write simulation output to ASCII file
 */
#define N_STATEVAR 4
#define MESH_DIM 3
#define N_NODESPERCELL 4

void WriteRaw(const char* fileName, int nnode, int ncell, int nedge, int nbedge, double* x, int* cell, 
	int* edge, int* ecell, int* bedge, int* becell, int* bound, double* q) {
	FILE* fp;
	if ((fp = fopen(fileName, "w")) == NULL) {
		printf("can't open file\n");
		return;
	}
	writerawHeader(fp, nnode, ncell, nedge, nbedge);
	writerawNode(fp, nnode, x);
	writerawCell(fp, ncell, cell);
	writerawEdge(fp, nedge, edge, ecell);
	writerawBEdge(fp, nbedge, bedge, becell, bound);
	writerawQ(fp, ncell, q);
	fclose(fp);
}

void ReadRaw(const char* fileName, int* nnode, int* ncell, int* nedge, int* nbedge, double** x, int** cell, 
	int** edge, int** ecell, int** bedge, int** becell, int** bound, double** q) {
	FILE* fp;
	if ((fp = fopen(fileName, "r")) == NULL) {
		printf("can't open file\n");
		return;
	}
	fscanf(fp, "%d %d %d %d \n", nnode, ncell, nedge, nbedge);
	*x = (double*)malloc(sizeof(double) * (*nnode) * MESH_DIM);
	readrawNode(fp, *nnode, *x);
	*cell = (int*)malloc(sizeof(int) * (*ncell) * N_NODESPERCELL);
	readrawCell(fp, *ncell, *cell);
	*edge = (int*)malloc(sizeof(int) * (*nedge) * 2);
	*ecell = (int*)malloc(sizeof(int) * (*nedge) * 2);
	readrawEdge(fp, *nedge, *edge, *ecell);
	*bedge = (int*)malloc(sizeof(int) * (*nbedge) * 2);
	*becell = (int*)malloc(sizeof(int) * (*nbedge));
	*bound = (int*)malloc(sizeof(int) * (*nbedge));
	readrawBEdge(fp, *nbedge, *bedge, *becell, *bound);
	*q = (double*)malloc(sizeof(double) * (*ncell) * N_STATEVAR);
	readrawQ(fp, *ncell, *q);
	fclose(fp);
}


void WriteMeshToVTKAscii(const char* filename, double* nodeCoords_data, int nnode, int* cellsToNodes_data, int ncell, double* values_data) {
	printf("Writing OutputSimulation to ASCII file: %s \n", filename);
	FILE* fp;
	fp = fopen(filename, "w");
	if (fp == NULL) {
		printf("can't open file for write %s\n", filename);
		exit(-1);
	}

	// write header
	fprintf(fp, "# vtk DataFile Version 2.0\n Output from OP2 Volna.\n");
	fprintf(fp, "ASCII \nDATASET UNSTRUCTURED_GRID\n\n");
	// write vertices
	fprintf(fp, "POINTS %d double\n", nnode);
	//  double* nodeCoords_data;
	//  nodeCoords_data = (double*)nodeCoords->data;
	int i = 0;
	for (i = 0; i < nnode; ++i) {
		fprintf(fp, "%g %g %g \n",
			(double)nodeCoords_data[i * MESH_DIM],
			(double)nodeCoords_data[i * MESH_DIM + 1],
			0.0);
	}
	fprintf(fp, "\n");
	fprintf(fp, "CELLS %d %d\n", ncell, 5 * ncell);
	for (i = 0; i < ncell; ++i) {
		fprintf(fp, "4 %d %d %d %d\n",
			cellsToNodes_data[i * N_NODESPERCELL],
			cellsToNodes_data[i * N_NODESPERCELL + 1],
			cellsToNodes_data[i * N_NODESPERCELL + 2],
			cellsToNodes_data[i * N_NODESPERCELL + 3]);
	}
	fprintf(fp, "\n");
	// write cell types (5 for triangles, 9 for quads)
	fprintf(fp, "CELL_TYPES %d\n", ncell);
	for (i = 0; i < ncell; ++i)
		fprintf(fp, "9\n");
	fprintf(fp, "\n");

	//  double* values_data;
	//  values_data = (double*) values->data;

	fprintf(fp, "CELL_DATA %d\n"
		"SCALARS q0 double 1\n"
		"LOOKUP_TABLE default\n",
		ncell);
	double tmp = 0.0;
	for (i = 0; i < ncell; ++i) {
		tmp = values_data[i * N_STATEVAR];
		fprintf(fp, "%10.20g\n", values_data[i * N_STATEVAR]);
	}

	fprintf(fp, "\n");
	fprintf(fp, "SCALARS q1 double 1\n"
		"LOOKUP_TABLE default\n");
	for (i = 0; i < ncell; ++i)
		fprintf(fp, "%10.20g\n", values_data[i * N_STATEVAR + 1]);
	fprintf(fp, "\n");
	fprintf(fp, "SCALARS q2 double 1\n"
		"LOOKUP_TABLE default\n");
	for (i = 0; i < ncell; ++i)
		fprintf(fp, "%10.20g\n", values_data[i * N_STATEVAR + 2]);
	fprintf(fp, "\n");
	fprintf(fp, "SCALARS q3 double 1\n"
		"LOOKUP_TABLE default\n");
	for (i = 0; i < ncell; ++i)
		fprintf(fp, "%10.20g\n", values_data[i * N_STATEVAR + 3]);
	fprintf(fp, "\n");

	if (fclose(fp) != 0) {
		printf("can't close file %s\n", filename);
		exit(-1);
	}
}




#endif // CodeExecutable

