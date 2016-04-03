// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "JavascriptIsolate.h"
#include "MyClass.generated.h"

class FDarknetNetwork_Private;
class FDarknetMatrix_Private;
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

	UPROPERTY()
	EDarknetActivation activation;

	UPROPERTY()
	EDarknetCost cost_type;
	
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
	static FDarknetMatrix make(int32 rows, int32 cols);

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
};
