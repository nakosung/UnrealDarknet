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

class FDarknetImage_Private
{
public:
	FDarknetImage_Private(image image)
		: image(image)
	{}

	~FDarknetImage_Private()
	{
		free_image(image);
	}

	image image;
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

FDarknetImage from_raw(image image)
{
	FDarknetImage instance;
	instance.Handle = TSharedPtr<FDarknetImage_Private>(new FDarknetImage_Private(image));
	return instance;
}

void* FDarknetMatrix::GetMemory(const int32* Dim)
{
	return Handle->mat.vals[Dim[0]];
}

int32 FDarknetMatrix::GetSize(int32 Dim)
{
	return Dim == 0 ? Handle->mat.rows : (Handle->mat.cols * sizeof(float));
}

void* FDarknetImage::GetMemory(const int32* Dim)
{
	return Handle->image.data;
}

int32 FDarknetImage::GetSize(int32 Dim)
{
	return Handle->image.w * Handle->image.h * Handle->image.c * sizeof(float);
}

FDarknetNetwork UMyClass::parse_network_cfg(const FString& Filename)
{
	FDarknetNetwork instance;	
	instance.Handle = TSharedPtr<FDarknetNetwork_Private>(new FDarknetNetwork_Private(Filename));
	return instance;
}

FDarknetMatrix UMyClass::make_matrix(int32 rows, int32 cols)
{
	return from_raw(::make_matrix(rows, cols));
}

FDarknetImage UMyClass::make_image(int32 w, int32 h, int32 c)
{
	return from_raw(::make_image(w, h, c));
}

FDarknetImage UMyClass::load_image(UTextureRenderTarget* Target)
{
	FTextureRenderTargetResource* RenderTarget = Target->GameThread_GetRenderTargetResource();

	auto Width = (int)Target->GetSurfaceWidth();
	auto Height = (int)Target->GetSurfaceHeight();
	auto image = ::make_image(Width, Height, 3);

	TArray<FColor> SurfaceData;
	SurfaceData.AddUninitialized(Width * Height);

	// Read pixels
	FIntRect Area(0, 0, Width, Height);
	auto readSurfaceDataFlags = FReadSurfaceDataFlags();
	readSurfaceDataFlags.SetLinearToGamma(false);
	RenderTarget->ReadPixelsPtr(SurfaceData.GetData(), readSurfaceDataFlags, Area);

	for (auto Y = 0; Y < Height; ++Y)
	{
		for (auto X = 0; X < Width; ++X)
		{
			const auto& Source = SurfaceData[X + Y * Width];
			::set_pixel(image, X, Y, 0, Source.R / 255.0f);
			::set_pixel(image, X, Y, 1, Source.G / 255.0f);
			::set_pixel(image, X, Y, 2, Source.B / 255.0f);
		}
	}
	return from_raw(image);
}

FDarknetImage UMyClass::copy_image(FDarknetImage p)
{
	return from_raw(::copy_image(p.Handle->image));
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

float UMyClass::get_color(int32 c, int32 x, int32 max)
{
	return ::get_color(c, x, max);
}
void UMyClass::flip_image(FDarknetImage a)
{
	return ::flip_image(a.Handle->image);
}
/*void draw_box(FDarknetImage a, int32 x1, int32 y1, int32 x2, int32 y2, float r, float g, float b);
void draw_box_width(FDarknetImage a, int32 x1, int32 y1, int32 x2, int32 y2, int32 w, float r, float g, float b);
void draw_bbox(FDarknetImage a, box bbox, int32 w, float r, float g, float b);
void draw_label(FDarknetImage a, int32 r, int32 c, FDarknetImage label, const float *rgb);
void draw_detections(FDarknetImage im, int32 num, float thresh, box *boxes, float **probs, char **names, FDarknetImage *labels, int32 classes);*/
FDarknetImage UMyClass::image_distance(FDarknetImage a, FDarknetImage b)
{
	return from_raw(::image_distance(a.Handle->image, b.Handle->image));
}
void UMyClass::scale_image(FDarknetImage m, float s)
{
	return ::scale_image(m.Handle->image, s);
}
FDarknetImage UMyClass::crop_image(FDarknetImage im, int32 dx, int32 dy, int32 w, int32 h)
{
	return from_raw(::crop_image(im.Handle->image, dx, dy, w, h));
}
FDarknetImage UMyClass::random_crop_image(FDarknetImage im, int32 low, int32 high, int32 size)
{
	return from_raw(::random_crop_image(im.Handle->image, low, high, size));
}
FDarknetImage UMyClass::resize_image(FDarknetImage im, int32 w, int32 h)
{
	return from_raw(::resize_image(im.Handle->image, w, h));
}
FDarknetImage UMyClass::resize_min(FDarknetImage im, int32 min)
{
	return from_raw(::resize_min(im.Handle->image, min));
}
void UMyClass::translate_image(FDarknetImage m, float s)
{
	::translate_image(m.Handle->image, s);
}
void UMyClass::normalize_image(FDarknetImage p)
{
	::normalize_image(p.Handle->image);
}
FDarknetImage UMyClass::rotate_image(FDarknetImage m, float rad)
{
	return from_raw(::rotate_image(m.Handle->image, rad));
}
void UMyClass::rotate_image_cw(FDarknetImage im, int32 times)
{
	::rotate_image_cw(im.Handle->image, times);
}
void UMyClass::embed_image(FDarknetImage source, FDarknetImage dest, int32 dx, int32 dy)
{
	::embed_image(source.Handle->image, dest.Handle->image, dx, dy);
}
void UMyClass::saturate_image(FDarknetImage im, float sat)
{
	::saturate_image(im.Handle->image, sat);
}
void UMyClass::exposure_image(FDarknetImage im, float sat)
{
	::exposure_image(im.Handle->image, sat);
}
void UMyClass::saturate_exposure_image(FDarknetImage im, float sat, float exposure)
{
	::saturate_exposure_image(im.Handle->image, sat, exposure);
}
void UMyClass::hsv_to_rgb(FDarknetImage im)
{
	::hsv_to_rgb(im.Handle->image);
}
void UMyClass::rgbgr_image(FDarknetImage im)
{
	::rgbgr_image(im.Handle->image);
}
void UMyClass::constrain_image(FDarknetImage im)
{
	::constrain_image(im.Handle->image);
}

FDarknetImage UMyClass::grayscale_image(FDarknetImage im)
{
	return from_raw(::grayscale_image(im.Handle->image));
}

FDarknetImage UMyClass::threshold_image(FDarknetImage im, float thresh)
{
	return from_raw(::threshold_image(im.Handle->image, thresh));
}

FDarknetImage UMyClass::collapse_image_layers(FDarknetImage source, int32 border)
{
	return from_raw(::collapse_image_layers(source.Handle->image, border));
}
FDarknetImage UMyClass::collapse_images_horz(FDarknetImage ims, int32 n)
{
	return from_raw(::collapse_images_horz(&ims.Handle->image, n));
}
FDarknetImage UMyClass::collapse_images_vert(FDarknetImage ims, int32 n)
{
	return from_raw(::collapse_images_vert(&ims.Handle->image, n));
}

float UMyClass::get_pixel(FDarknetImage m, int32 x, int32 y, int32 c)
{
	return ::get_pixel(m.Handle->image, x, y, c);
}
float UMyClass::get_pixel_extend(FDarknetImage m, int32 x, int32 y, int32 c)
{
	return ::get_pixel_extend(m.Handle->image, x, y, c);
}
void UMyClass::set_pixel(FDarknetImage m, int32 x, int32 y, int32 c, float val)
{
	::set_pixel(m.Handle->image, x, y, c, val);
}
void UMyClass::add_pixel(FDarknetImage m, int32 x, int32 y, int32 c, float val)
{
	::add_pixel(m.Handle->image, x, y, c, val);
}
float UMyClass::bilinear_interpolate(FDarknetImage im, float x, float y, int32 c)
{
	return ::bilinear_interpolate(im.Handle->image, x, y, c);
}