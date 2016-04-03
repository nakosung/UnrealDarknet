// Fill out your copyright notice in the Description page of Project Settings.

#include "darknet.h"
#include "MyClass.h"

PRAGMA_DISABLE_OPTIMIZATION

extern "C" {
	#include <darknet/src/network.h>
	#include <darknet/src/parser.h>
}

class FDarknetNetwork_Private 
{
public:
	FDarknetNetwork_Private(const FString& Filename)
	{
		net = parse_network_cfg(TCHAR_TO_ANSI(*Filename));
	}

	~FDarknetNetwork_Private()
	{
		free_network(net);
	}

	network net;
};

class FDarknetMatrix_Private
{
public:
	FDarknetMatrix_Private(matrix m)
		: mat(m)
	{}

	~FDarknetMatrix_Private()
	{
		free_matrix(mat);
	}

	matrix mat;
};

class FDarknetData_Private
{
public:
	FDarknetData_Private(data d)
		: data(d)
	{}

	~FDarknetData_Private()
	{
		free_data(data);
	}

	data data;
};

FDarknetMatrix from_raw(matrix mat)
{
	FDarknetMatrix instance;
	instance.Handle = TSharedPtr<FDarknetMatrix_Private>(new FDarknetMatrix_Private(mat));
	return instance;
}

FDarknetMatrix from_raw(int cols, float* buf)
{
	auto mat = make_matrix(1, cols);
	free(mat.vals[0]);
	mat.vals[0] = buf;
	return from_raw(mat);
}

void* FDarknetMatrix::GetMemory(const int32* Dim)
{
	return Handle->mat.vals[Dim[0]];
}

int32 FDarknetMatrix::GetSize(int32 Dim)
{
	return Dim == 0 ? Handle->mat.rows : (Handle->mat.cols * sizeof(float));
}

FDarknetNetwork UMyClass::parse_network_cfg(const FString& Filename)
{
	FDarknetNetwork instance;	
	instance.Handle = TSharedPtr<FDarknetNetwork_Private>(new FDarknetNetwork_Private(Filename));
	return instance;
}

FDarknetMatrix UMyClass::make(int32 rows, int32 cols)
{
	return from_raw(make_matrix(rows, cols));
}

void UMyClass::load_weights(FDarknetNetwork instance, const FString& Filename)
{
	::load_weights(&instance.Handle->net, TCHAR_TO_ANSI(*Filename));
}

int UMyClass::get_current_batch(FDarknetNetwork instance)
{
	return ::get_current_batch(instance.Handle->net);
}

float UMyClass::get_current_rate(FDarknetNetwork instance)
{
	return ::get_current_rate(instance.Handle->net);
}

void UMyClass::update_network(FDarknetNetwork instance)
{
	::update_network(instance.Handle->net);
}

FDarknetMatrix UMyClass::hold_out_matrix(FDarknetMatrix m, int n)
{
	return from_raw(::hold_out_matrix(&m.Handle->mat, n));
}

float UMyClass::matrix_topk_accuracy(FDarknetMatrix truth, FDarknetMatrix guess, int32 k)
{
	return ::matrix_topk_accuracy(truth.Handle->mat, guess.Handle->mat, k);
}

void UMyClass::matrix_add_matrix(FDarknetMatrix from, FDarknetMatrix to)
{
	::matrix_add_matrix(from.Handle->mat, to.Handle->mat);
}

void UMyClass::scale_matrix(FDarknetMatrix m, float scale)
{
	::scale_matrix(m.Handle->mat, scale);
}

FDarknetMatrix UMyClass::resize_matrix(FDarknetMatrix m, int32 size)
{
	return from_raw(::resize_matrix(m.Handle->mat, size));
}

FDarknetMatrix UMyClass::pop_column(FDarknetMatrix m, int32 c)
{
	auto rows = m.Handle->mat.rows;
	return from_raw(rows, ::pop_column(&m.Handle->mat, c));	
}

float UMyClass::train_network(FDarknetNetwork net, FDarknetData d)
{
	return ::train_network(net.Handle->net, d.Handle->data);
}

float UMyClass::train_network_batch(FDarknetNetwork net, FDarknetData d, int n)
{
	return ::train_network_batch(net.Handle->net, d.Handle->data, n);
}

float UMyClass::train_network_sgd(FDarknetNetwork net, FDarknetData d, int n)
{
	return ::train_network_sgd(net.Handle->net, d.Handle->data, n);
}

FDarknetMatrix UMyClass::network_predict_data(FDarknetNetwork net, FDarknetData test)
{
	return from_raw(::network_predict_data(net.Handle->net, test.Handle->data));
}

FDarknetMatrix UMyClass::network_predict(FDarknetNetwork net, FDarknetMatrix input)
{
	auto size = get_network_output_size(net);
	auto buf = malloc(size * sizeof(float));
	auto out = ::network_predict(net.Handle->net, input.Handle->mat.vals[0]);
	FMemory::Memcpy(buf, out, sizeof(float) * size);
	return from_raw(size,(float*)buf);
}

float UMyClass::network_accuracy(FDarknetNetwork net, FDarknetData d)
{
	return ::network_accuracy(net.Handle->net, d.Handle->data);
}

float UMyClass::network_accuracy_multi(FDarknetNetwork net, FDarknetData d, int32 n)
{
	return ::network_accuracy_multi(net.Handle->net, d.Handle->data, n);
}

int32 UMyClass::get_network_output_size(FDarknetNetwork net)
{
	return ::get_network_output_size(net.Handle->net);
}
int32 UMyClass::get_predicted_class_network(FDarknetNetwork net)
{
	return ::get_predicted_class_network(net.Handle->net);
}
int32 UMyClass::resize_network(FDarknetNetwork net, int32 w, int32 h)
{
	return ::resize_network(&net.Handle->net, w, h);
}
void UMyClass::set_batch_network(FDarknetNetwork net, int32 b)
{
	::set_batch_network(&net.Handle->net, b);
}
int32 UMyClass::get_network_input_size(FDarknetNetwork net)
{
	return ::get_network_input_size(net.Handle->net);
}
float UMyClass::get_network_cost(FDarknetNetwork net)
{
	return ::get_network_cost(net.Handle->net);
}

UScriptStruct* FDarknetNetwork::GetScriptStruct(int32 Index)
{
	auto& net = Handle->net;

	if (Index == 0) return FDarknetNetwork_Raw::StaticStruct();
	Index--;

	if (Index < net.n) return FDarknetLayer_Raw::StaticStruct();
	Index -= net.n;

	return nullptr;
}

void* FDarknetNetwork::GetData(int32 Index)
{
	auto& net = Handle->net;

	if (Index == 0) return &net;
	Index--;

	if (Index < net.n) return net.layers + Index;
	Index -= net.n;

	if (Index < net.outputs) return net.output + Index;
	Index -= net.outputs;

	return nullptr;
}

int32 FDarknetNetwork::GetNumData()
{
	auto& net = Handle->net;

	return 1 + net.n + net.outputs;
}

FName FDarknetNetwork::GetDataName(int32 Index)
{
	auto& net = Handle->net;
	if (Index == 0) return FName("self");
	Index--;

	if (Index < net.n) return FName("layer", Index+1);
	Index -= net.n;

	if (Index < net.outputs) return FName("output", Index+1);
	Index -= net.outputs;
	
	return FName();
}