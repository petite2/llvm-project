// RUN: %clang_cc1 -triple x86_64-apple-darwin %s -emit-llvm -disable-llvm-passes -o - | FileCheck %s

typedef float float4 __attribute__((ext_vector_type(4)));
typedef short int si8 __attribute__((ext_vector_type(8)));
typedef unsigned int u4 __attribute__((ext_vector_type(4)));

__attribute__((address_space(1))) int int_as_one;
typedef int bar;
bar b;

void test_builtin_elementwise_max(float f1, float f2, double d1, double d2,
                                  float4 vf1, float4 vf2, long long int i1,
                                  long long int i2, si8 vi1, si8 vi2,
                                  unsigned u1, unsigned u2, u4 vu1, u4 vu2) {
  // CHECK-LABEL: define void @test_builtin_elementwise_max(

  // CHECK:      [[F1:%.+]] = load float, float* %f1.addr, align 4
  // CHECK-NEXT: [[F2:%.+]] = load float, float* %f2.addr, align 4
  // CHECK-NEXT:  call float @llvm.maxnum.f32(float %0, float %1)
  f1 = __builtin_elementwise_max(f1, f2);

  // CHECK:      [[D1:%.+]] = load double, double* %d1.addr, align 8
  // CHECK-NEXT: [[D2:%.+]] = load double, double* %d2.addr, align 8
  // CHECK-NEXT: call double @llvm.maxnum.f64(double [[D1]], double [[D2]])
  d1 = __builtin_elementwise_max(d1, d2);

  // CHECK:      [[D2:%.+]] = load double, double* %d2.addr, align 8
  // CHECK-NEXT: call double @llvm.maxnum.f64(double 2.000000e+01, double [[D2]])
  d1 = __builtin_elementwise_max(20.0, d2);

  // CHECK:      [[VF1:%.+]] = load <4 x float>, <4 x float>* %vf1.addr, align 16
  // CHECK-NEXT: [[VF2:%.+]] = load <4 x float>, <4 x float>* %vf2.addr, align 16
  // CHECK-NEXT: call <4 x float> @llvm.maxnum.v4f32(<4 x float> [[VF1]], <4 x float> [[VF2]])
  vf1 = __builtin_elementwise_max(vf1, vf2);

  // CHECK:      [[I1:%.+]] = load i64, i64* %i1.addr, align 8
  // CHECK-NEXT: [[I2:%.+]] = load i64, i64* %i2.addr, align 8
  // CHECK-NEXT: call i64 @llvm.smax.i64(i64 [[I1]], i64 [[I2]])
  i1 = __builtin_elementwise_max(i1, i2);

  // CHECK:      [[I1:%.+]] = load i64, i64* %i1.addr, align 8
  // CHECK-NEXT: call i64 @llvm.smax.i64(i64 [[I1]], i64 10)
  i1 = __builtin_elementwise_max(i1, 10);

  // CHECK:      [[VI1:%.+]] = load <8 x i16>, <8 x i16>* %vi1.addr, align 16
  // CHECK-NEXT: [[VI2:%.+]] = load <8 x i16>, <8 x i16>* %vi2.addr, align 16
  // CHECK-NEXT: call <8 x i16> @llvm.smax.v8i16(<8 x i16> [[VI1]], <8 x i16> [[VI2]])
  vi1 = __builtin_elementwise_max(vi1, vi2);

  // CHECK:      [[U1:%.+]] = load i32, i32* %u1.addr, align 4
  // CHECK-NEXT: [[U2:%.+]] = load i32, i32* %u2.addr, align 4
  // CHECK-NEXT: call i32 @llvm.umax.i32(i32 [[U1]], i32 [[U2]])
  u1 = __builtin_elementwise_max(u1, u2);

  // CHECK:      [[VU1:%.+]] = load <4 x i32>, <4 x i32>* %vu1.addr, align 16
  // CHECK-NEXT: [[VU2:%.+]] = load <4 x i32>, <4 x i32>* %vu2.addr, align 16
  // CHECK-NEXT: call <4 x i32> @llvm.umax.v4i32(<4 x i32> [[VU1]], <4 x i32> [[VU2]])
  vu1 = __builtin_elementwise_max(vu1, vu2);

  // CHECK:      [[CVF1:%.+]] = load <4 x float>, <4 x float>* %cvf1, align 16
  // CHECK-NEXT: [[VF2:%.+]] = load <4 x float>, <4 x float>* %vf2.addr, align 16
  // CHECK-NEXT: call <4 x float> @llvm.maxnum.v4f32(<4 x float> [[CVF1]], <4 x float> [[VF2]])
  const float4 cvf1 = vf1;
  vf1 = __builtin_elementwise_max(cvf1, vf2);

  // CHECK:      [[VF2:%.+]] = load <4 x float>, <4 x float>* %vf2.addr, align 16
  // CHECK-NEXT: [[CVF1:%.+]] = load <4 x float>, <4 x float>* %cvf1, align 16
  // CHECK-NEXT: call <4 x float> @llvm.maxnum.v4f32(<4 x float> [[VF2]], <4 x float> [[CVF1]])
  vf1 = __builtin_elementwise_max(vf2, cvf1);

  // CHECK:      [[IAS1:%.+]] = load i32, i32 addrspace(1)* @int_as_one, align 4
  // CHECK-NEXT: [[B:%.+]] = load i32, i32* @b, align 4
  // CHECK-NEXT: call i32 @llvm.smax.i32(i32 [[IAS1]], i32 [[B]])
  int_as_one = __builtin_elementwise_max(int_as_one, b);

  // CHECK: call i32 @llvm.smax.i32(i32 1, i32 97)
  i1 = __builtin_elementwise_max(1, 'a');
}

void test_builtin_elementwise_min(float f1, float f2, double d1, double d2,
                                  float4 vf1, float4 vf2, long long int i1,
                                  long long int i2, si8 vi1, si8 vi2,
                                  unsigned u1, unsigned u2, u4 vu1, u4 vu2) {
  // CHECK-LABEL: define void @test_builtin_elementwise_min(
  // CHECK:      [[F1:%.+]] = load float, float* %f1.addr, align 4
  // CHECK-NEXT: [[F2:%.+]] = load float, float* %f2.addr, align 4
  // CHECK-NEXT:  call float @llvm.minnum.f32(float %0, float %1)
  f1 = __builtin_elementwise_min(f1, f2);

  // CHECK:      [[D1:%.+]] = load double, double* %d1.addr, align 8
  // CHECK-NEXT: [[D2:%.+]] = load double, double* %d2.addr, align 8
  // CHECK-NEXT: call double @llvm.minnum.f64(double [[D1]], double [[D2]])
  d1 = __builtin_elementwise_min(d1, d2);

  // CHECK:      [[D1:%.+]] = load double, double* %d1.addr, align 8
  // CHECK-NEXT: call double @llvm.minnum.f64(double [[D1]], double 2.000000e+00)
  d1 = __builtin_elementwise_min(d1, 2.0);

  // CHECK:      [[VF1:%.+]] = load <4 x float>, <4 x float>* %vf1.addr, align 16
  // CHECK-NEXT: [[VF2:%.+]] = load <4 x float>, <4 x float>* %vf2.addr, align 16
  // CHECK-NEXT: call <4 x float> @llvm.minnum.v4f32(<4 x float> [[VF1]], <4 x float> [[VF2]])
  vf1 = __builtin_elementwise_min(vf1, vf2);

  // CHECK:      [[I1:%.+]] = load i64, i64* %i1.addr, align 8
  // CHECK-NEXT: [[I2:%.+]] = load i64, i64* %i2.addr, align 8
  // CHECK-NEXT: call i64 @llvm.smin.i64(i64 [[I1]], i64 [[I2]])
  i1 = __builtin_elementwise_min(i1, i2);

  // CHECK:      [[I2:%.+]] = load i64, i64* %i2.addr, align 8
  // CHECK-NEXT: call i64 @llvm.smin.i64(i64 -11, i64 [[I2]])
  i1 = __builtin_elementwise_min(-11, i2);

  // CHECK:      [[VI1:%.+]] = load <8 x i16>, <8 x i16>* %vi1.addr, align 16
  // CHECK-NEXT: [[VI2:%.+]] = load <8 x i16>, <8 x i16>* %vi2.addr, align 16
  // CHECK-NEXT: call <8 x i16> @llvm.smin.v8i16(<8 x i16> [[VI1]], <8 x i16> [[VI2]])
  vi1 = __builtin_elementwise_min(vi1, vi2);

  // CHECK:      [[U1:%.+]] = load i32, i32* %u1.addr, align 4
  // CHECK-NEXT: [[U2:%.+]] = load i32, i32* %u2.addr, align 4
  // CHECK-NEXT: call i32 @llvm.umin.i32(i32 [[U1]], i32 [[U2]])
  u1 = __builtin_elementwise_min(u1, u2);

  // CHECK:      [[U1:%.+]] = load i32, i32* %u1.addr, align 4
  // CHECK-NEXT: [[ZEXT_U1:%.+]] = zext i32 [[U1]] to i64
  // CHECK-NEXT: [[I2:%.+]] = load i64, i64* %i2.addr, align 8
  // CHECK-NEXT: call i64 @llvm.smin.i64(i64 [[ZEXT_U1]], i64 [[I2]])
  u1 = __builtin_elementwise_min(u1, i2);

  // CHECK:      [[VU1:%.+]] = load <4 x i32>, <4 x i32>* %vu1.addr, align 16
  // CHECK-NEXT: [[VU2:%.+]] = load <4 x i32>, <4 x i32>* %vu2.addr, align 16
  // CHECK-NEXT: call <4 x i32> @llvm.umin.v4i32(<4 x i32> [[VU1]], <4 x i32> [[VU2]])
  vu1 = __builtin_elementwise_min(vu1, vu2);

  // CHECK:      [[CVF1:%.+]] = load <4 x float>, <4 x float>* %cvf1, align 16
  // CHECK-NEXT: [[VF2:%.+]] = load <4 x float>, <4 x float>* %vf2.addr, align 16
  // CHECK-NEXT: call <4 x float> @llvm.minnum.v4f32(<4 x float> [[CVF1]], <4 x float> [[VF2]])
  const float4 cvf1 = vf1;
  vf1 = __builtin_elementwise_min(cvf1, vf2);

  // CHECK:      [[VF2:%.+]] = load <4 x float>, <4 x float>* %vf2.addr, align 16
  // CHECK-NEXT: [[CVF1:%.+]] = load <4 x float>, <4 x float>* %cvf1, align 16
  // CHECK-NEXT: call <4 x float> @llvm.minnum.v4f32(<4 x float> [[VF2]], <4 x float> [[CVF1]])
  vf1 = __builtin_elementwise_min(vf2, cvf1);

  // CHECK:      [[IAS1:%.+]] = load i32, i32 addrspace(1)* @int_as_one, align 4
  // CHECK-NEXT: [[B:%.+]] = load i32, i32* @b, align 4
  // CHECK-NEXT: call i32 @llvm.smin.i32(i32 [[IAS1]], i32 [[B]])
  int_as_one = __builtin_elementwise_min(int_as_one, b);
}
