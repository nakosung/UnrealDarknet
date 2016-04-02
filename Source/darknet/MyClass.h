// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyClass.generated.h"

class FDarknetNetwork_Private;
class FDarknetMatrix_Private;
class FDarknetData_Private;

USTRUCT()
struct FDarknetMatrix
{
	GENERATED_BODY()

	TSharedPtr<FDarknetMatrix_Private> Handle;
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


USTRUCT(BlueprintType)
struct FDarknetNetwork
{
	GENERATED_BODY();

	TSharedPtr<FDarknetNetwork_Private> Handle;
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
