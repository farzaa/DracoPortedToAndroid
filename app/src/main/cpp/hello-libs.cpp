/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <cstring>
#include <jni.h>
#include <cinttypes>


#include <cinttypes>
#include <fstream>
#include "draco/compression/decode.h"
#include "draco/core/cycle_timer.h"
#include "draco/io/obj_encoder.h"
#include "draco/io/parser_utils.h"
#include "draco/io/ply_encoder.h"
#include <unistd.h>

#include <syslog.h>
#include <android/log.h>


#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, "hello-libs::", __VA_ARGS__))

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   app/src/main/java/com/example/hellolibs/MainActivity.java
 */
extern "C" JNIEXPORT void JNICALL


Java_com_example_hellolibs_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {

//    LOGI("%s", "✅ Welcome to Draco by Flynn.");

    std::ifstream input_file("/storage/emulated/0/FLYNN/BlueberryPancakes.drc", std::ios::binary);

    if (!input_file) {
//        LOGI("%s", "❌ Input file not found.");
        return;
    } else {
//        LOGI("%s", "✅ Opened input file.");
    }

    // Read the file stream into a buffer.
    std::streampos file_size = 0;
    input_file.seekg(0, std::ios::end);
    file_size = input_file.tellg() - file_size;
    input_file.seekg(0, std::ios::beg);
    std::vector<char> data(file_size);
    input_file.read(data.data(), file_size);

    if (data.empty()) {
//        LOGI("%s", "❌ Input file was empty");
        return;
    } else {
//        LOGI("%s", "✅ Input file was not empty!");
    }

    draco::DecoderBuffer buffer;
    buffer.Init(data.data(), data.size());

    // Decode the input data into a geometry.
    std::unique_ptr<draco::PointCloud> pc;
    draco::Mesh *mesh = nullptr;
    auto type_statusor = draco::Decoder::GetEncodedGeometryType(&buffer);
    if (!type_statusor.ok()) {
//        LOGI("%s", "❌ Decoded geometry type is messed up!");
        return;
    }


//    LOGI("ANOTHER... %s", type_statusor.status().error_msg());
    const draco::EncodedGeometryType geom_type = type_statusor.value();
    if (geom_type == draco::TRIANGULAR_MESH) {

        draco::Decoder decoder;
        auto statusor = decoder.DecodeMeshFromBuffer(&buffer);
//        LOGI("%s", statusor.status().error_msg());
        if (!statusor.ok()) {
//            LOGI("%s", "❌ Bad decode in triangular mesh\n");
            return;
        }

        std::unique_ptr<draco::Mesh> in_mesh = std::move(statusor).value();

        if (in_mesh) {
            mesh = in_mesh.get();
            pc = std::move(in_mesh);
        }
    } else if (geom_type == draco::POINT_CLOUD) {
        // Failed to decode it as mesh, so let's try to decode it as a point cloud.
        draco::Decoder decoder;
        auto statusor = decoder.DecodePointCloudFromBuffer(&buffer);
        if (!statusor.ok()) {
//            printf("%s", "❌ Bad decode in P.C mesh \n");
            LOGI("%s", "❌ Bad decode in P.C mesh\n");
            return;
        }
        pc = std::move(statusor).value();
    }

    if (pc == nullptr) {
        LOGI("%s", "❌ Failed to decode the input file.\n");
    } else {
        LOGI("%s", "✅ Decoded!");
    }

    const std::string extension = draco::parser::ToLower(".obj");
    const std::string outPath = "/storage/emulated/0/FLYNN/fuckYourMom.obj";

    if (extension == ".obj") {
        draco::ObjEncoder obj_encoder;
        if (mesh) {
            if (!obj_encoder.EncodeToFile(*mesh, outPath)) {
                LOGI("%s", "❌ Failed to store as an OBJ!!");

            }
        } else {
            if (!obj_encoder.EncodeToFile(*pc.get(), outPath)) {
                LOGI("%s", "❌ Failed to store as an OBJ (2)!!");

            }
        }
    }

}
