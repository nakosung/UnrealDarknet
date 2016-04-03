// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "JavascriptIsolate.h"
#include "MyClass.generated.h"

class FDarknetNetwork_Private;
class FDarknetMatrix_Private;
class FDarknetImage_Private;
class FDarknetData_Private;

USTRUCT()
struct FDarknetMatrix : public FJavascriptMemoryStruct
{
	GENERATED_BODY()

	TSharedPtr<FDarknetMatrix_Private> Handle;

public:
	virtual int32 GetDimension() override { return 2; }
	virtual void* GetMemory(const int32* Dim) override;
	virtual int32 GetSize(int32 Dim) override;
};

USTRUCT()
struct FDarknetData
{
	GENERATED_BODY()

	TSharedPtr<FDarknetData_Private> Handle;
};

USTRUCT()
struct FDarknetImage : public FJavascriptMemoryStruct
{
	GENERATED_BODY()

	TSharedPtr<FDarknetImage_Private> Handle;

public:
	virtual int32 GetDimension() override { return 1; }
	virtual void* GetMemory(const int32* Dim) override;
	virtual int32 GetSize(int32 Dim) override;
};

UENUM()
enum class EDarknetActivation : uint8 {
	LOGISTIC, RELU, RELIE, LINEAR, RAMP, TANH, PLSE, LEAKY, ELU, LOGGY
};

UENUM()
enum class EDarknetLayer : uint8 {
	CONVOLUTIONAL,
	DECONVOLUTIONAL,
	CONNECTED,
	MAXPOOL,
	SOFTMAX,
	DETECTION,
	DROPOUT,
	CROP,
	ROUTE,
	COST,
	NORMALIZATION,
	AVGPOOL,
	LOCAL,
	SHORTCUT,
	ACTIVE,
	RNN,
	CRNN
};

UENUM()
enum class EDarknetCost : uint8 {
	SSE, MASKED, SMOOTH
};

UENUM()
enum class EDarknetLearningRatePolicy : uint8 {
	CONSTANT, STEP, EXP, POLY, STEPS, SIG
};

USTRUCT()
struct FDarknetNetwork_Raw
{
	GENERATED_BODY();

public:
	UPROPERTY()
	int32 n;

	UPROPERTY()
	int32 batch;

	int *seen;

	UPROPERTY()
	float epoch;

	UPROPERTY()
	int32 subdivisions;

	UPROPERTY()
	float momentum;

	UPROPERTY()
	float decay;

	void* layers;

	UPROPERTY()
	int32 outputs;

	float *output;

	UPROPERTY()
	EDarknetLearningRatePolicy policy;

	uint8 pad0[3];

	UPROPERTY()
	float learning_rate;

	UPROPERTY()
	float gamma;

	UPROPERTY()
	float scale;

	UPROPERTY()
	float power;

	UPROPERTY()
	int32 time_steps;

	UPROPERTY()
	int32 step;

	UPROPERTY()
	int32 max_batches;

	float *scales;
	int   *steps;

	UPROPERTY()
	int32 num_steps;

	UPROPERTY()
	int32 inputs;

	UPROPERTY()
	int32 h;

	UPROPERTY()
	int32 w;

	UPROPERTY()
	int32 c;

	UPROPERTY()
	int32 max_crop;
};

USTRUCT()
struct FDarknetLayer_Raw
{
	GENERATED_BODY()

	UPROPERTY()
	EDarknetLayer type;

	uint8 pad0[3];

	UPROPERTY()
	EDarknetActivation activation;

	uint8 pad1[3];

	UPROPERTY()
	EDarknetCost cost_type;

	uint8 pad2[3];
	
	UPROPERTY()
	int batch_normalize;

	UPROPERTY()
	int shortcut;

	UPROPERTY()
	int batch;

	UPROPERTY()
	int forced;

	UPROPERTY()
	int flipped;

	UPROPERTY()
	int inputs;

	UPROPERTY()
	int outputs;

	UPROPERTY()
	int truths;

	UPROPERTY()
	int h;
	
	UPROPERTY()
	int w;
	
	UPROPERTY()
	int c;

	UPROPERTY()
	int out_h;
	
	UPROPERTY()
	int out_w;

	UPROPERTY()
	int out_c;

	UPROPERTY()
	int n;

	UPROPERTY()
	int groups;

	UPROPERTY()
	int size;

	UPROPERTY()
	int side;

	UPROPERTY()
	int stride;

	UPROPERTY()
	int pad;

	UPROPERTY()
	int sqrt;

	UPROPERTY()
	int flip;

	UPROPERTY()
	int index;

	UPROPERTY()
	int binary;

	UPROPERTY()
	int steps;

	UPROPERTY()
	int hidden;

	UPROPERTY()
	float dot;

	UPROPERTY()
	float angle;

	UPROPERTY()
	float jitter;

	UPROPERTY()
	float saturation;

	UPROPERTY()
	float exposure;

	UPROPERTY()
	float shift;

	UPROPERTY()
	int softmax;

	UPROPERTY()
	int classes;

	UPROPERTY()
	int coords;

	UPROPERTY()
	int background;

	UPROPERTY()
	int rescore;

	UPROPERTY()
	int objectness;

	UPROPERTY()
	int does_cost;

	UPROPERTY()
	int joint;

	UPROPERTY()
	int noadjust;

	UPROPERTY()
	float alpha;

	UPROPERTY()
	float beta;

	UPROPERTY()
	float kappa;

	UPROPERTY()
	float coord_scale;

	UPROPERTY()
	float object_scale;

	UPROPERTY()
	float noobject_scale;

	UPROPERTY()
	float class_scale;

	UPROPERTY()
	int dontload;

	UPROPERTY()
	int dontloadscales;

	UPROPERTY()
	float temperature;

	UPROPERTY()
	float probability;

	UPROPERTY()
	float scale;

	int *indexes;
	float *rand;
	float *cost;
	float *filters;
	char  *cfilters;
	float *filter_updates;
	float *state;

	float *binary_filters;

	float *biases;
	float *bias_updates;

	float *scales;
	float *scale_updates;

	float *weights;
	float *weight_updates;

	float *col_image;
	int   * input_layers;
	int   * input_sizes;
	float * delta;
	float * output;
	float * squared;
	float * norms;

	float * spatial_mean;
	float * mean;
	float * variance;

	float * mean_delta;
	float * variance_delta;

	float * rolling_mean;
	float * rolling_variance;

	float * x;
	float * x_norm;

	FDarknetLayer_Raw* input_layer;
	FDarknetLayer_Raw* self_layer;
	FDarknetLayer_Raw* output_layer;
};

USTRUCT(BlueprintType)
struct FDarknetNetwork : public FJavascriptRawAccess
{
	GENERATED_BODY();

	TSharedPtr<FDarknetNetwork_Private> Handle;

	virtual UScriptStruct* GetScriptStruct(int32 Index);
	virtual void* GetData(int32 Index);
	virtual int32 GetNumData();
	virtual FName GetDataName(int32 Index);
};

/**
 * 
 */
UCLASS()
class DARKNET_API UMyClass : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetNetwork parse_network_cfg(const FString& Filename);

	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetMatrix make_matrix(int32 rows, int32 cols);

	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetImage make_image(int32 w, int32 h, int32 c);

	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetImage load_image(int32 dummy, UTextureRenderTarget* Target);
	
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetImage copy_image(FDarknetImage p);

	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void load_weights(FDarknetNetwork net, const FString& Filename);

	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static int get_current_batch(FDarknetNetwork net);

	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static float get_current_rate(FDarknetNetwork net);

	/*UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void forward_network(network net, network_state state);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void backward_network(network net, network_state state);*/
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void update_network(FDarknetNetwork net);

	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static float train_network(FDarknetNetwork net, FDarknetData d);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static float train_network_batch(FDarknetNetwork net, FDarknetData d, int n);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static float train_network_sgd(FDarknetNetwork net, FDarknetData d, int n);
	/*UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static float train_network_datum(network net, float *x, float *y);*/

	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetMatrix network_predict_image(FDarknetNetwork net, FDarknetImage test);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetMatrix network_predict_data(FDarknetNetwork net, FDarknetData test);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetMatrix network_predict(FDarknetNetwork net, FDarknetMatrix input);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static float network_accuracy(FDarknetNetwork net, FDarknetData d);
	/*float *network_accuracies(network net, data d, int n);*/
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static float network_accuracy_multi(FDarknetNetwork net, FDarknetData d, int32 n);
	/*void top_predictions(network net, int n, int *index);
	float *get_network_output(network net);
	float *get_network_output_layer(network net, int i);
	float *get_network_delta_layer(network net, int i);
	float *get_network_delta(network net);
	*/
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static int32 get_network_output_size(FDarknetNetwork net);
	/*image get_network_image(network net);
	image get_network_image_layer(network net, int i);
	*/
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static int32 get_predicted_class_network(FDarknetNetwork net);
	/*void print_network(network net);
	void visualize_network(network net);
	*/
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static int32 resize_network(FDarknetNetwork net, int32 w, int32 h);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void set_batch_network(FDarknetNetwork net, int32 b);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static int32 get_network_input_size(FDarknetNetwork net);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static float get_network_cost(FDarknetNetwork net);

	/*
	int get_network_nuisance(network net);
	int get_network_background(network net);
*/

	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetMatrix hold_out_matrix(FDarknetMatrix m, int n);

	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static float matrix_topk_accuracy(FDarknetMatrix truth, FDarknetMatrix guess, int32 k);

	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void matrix_add_matrix(FDarknetMatrix from, FDarknetMatrix to);

	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void scale_matrix(FDarknetMatrix m, float scale);

	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetMatrix resize_matrix(FDarknetMatrix m, int32 size);

	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetMatrix pop_column(FDarknetMatrix m, int32 c);

	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static float get_color(int32 c, int32 x, int32 max);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void flip_image(FDarknetImage a);
	/*void draw_box(FDarknetImage a, int32 x1, int32 y1, int32 x2, int32 y2, float r, float g, float b);
	static void draw_box_width(FDarknetImage a, int32 x1, int32 y1, int32 x2, int32 y2, int32 w, float r, float g, float b);
	static void draw_bbox(FDarknetImage a, box bbox, int32 w, float r, float g, float b);
	static void draw_label(FDarknetImage a, int32 r, int32 c, FDarknetImage label, const float *rgb);
	static void draw_detections(FDarknetImage im, int32 num, float thresh, box *boxes, float **probs, char **names, FDarknetImage *labels, int32 classes);*/
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetImage image_distance(FDarknetImage a, FDarknetImage b);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void scale_image(FDarknetImage m, float s);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetImage crop_image(FDarknetImage im, int32 dx, int32 dy, int32 w, int32 h);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetImage random_crop_image(FDarknetImage im, int32 low, int32 high, int32 size);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetImage resize_image(FDarknetImage im, int32 w, int32 h);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetImage resize_min(FDarknetImage im, int32 min);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void translate_image(FDarknetImage m, float s);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void normalize_image(FDarknetImage p);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetImage rotate_image(FDarknetImage m, float rad);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void rotate_image_cw(FDarknetImage im, int32 times);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void embed_image(FDarknetImage source, FDarknetImage dest, int32 dx, int32 dy);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void saturate_image(FDarknetImage im, float sat);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void exposure_image(FDarknetImage im, float sat);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void saturate_exposure_image(FDarknetImage im, float sat, float exposure);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void hsv_to_rgb(FDarknetImage im);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void rgbgr_image(FDarknetImage im);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void constrain_image(FDarknetImage im);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetImage grayscale_image(FDarknetImage im);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetImage threshold_image(FDarknetImage im, float thresh);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetImage collapse_image_layers(FDarknetImage source, int32 border);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetImage collapse_images_horz(FDarknetImage ims, int32 n);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static FDarknetImage collapse_images_vert(FDarknetImage ims, int32 n);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static float get_pixel(FDarknetImage m, int32 x, int32 y, int32 c);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static float get_pixel_extend(FDarknetImage m, int32 x, int32 y, int32 c);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void set_pixel(FDarknetImage m, int32 x, int32 y, int32 c, float val);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static void add_pixel(FDarknetImage m, int32 x, int32 y, int32 c, float val);
	UFUNCTION(BlueprintCallable, Category = "Machine learning | Dark net")
	static float bilinear_interpolate(FDarknetImage im, float x, float y, int32 c);
};
