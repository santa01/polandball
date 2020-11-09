/*
 * Copyright (c) 2013 Pavlo Lavrenenko
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <PolandBall.h>
#include <ArgumentParser.h>
#include <Config.h>
#include <EngineConfig.h>

int main(int argc, char** argv) {
    PolandBall::ArgumentParser arguments;
    arguments.setDescription(POLANDBALL_DESCRIPTION);
    arguments.setVersion(POLANDBALL_VERSION);

    arguments.addArgument('f', "fov", "camera field of view", PolandBall::ValueType::FLOAT);
    arguments.addArgument('h', "height", "viewport height", PolandBall::ValueType::INT);
    arguments.addArgument('w', "width", "viewport width", PolandBall::ValueType::INT);
    arguments.addArgument('s', "samples", "MSAA samples", PolandBall::ValueType::INT);
    arguments.addArgument('F', "fps", "maximum fps limit", PolandBall::ValueType::FLOAT);
    arguments.addArgument('v', "vsync", "vertical sync", PolandBall::ValueType::BOOL);
    arguments.addArgument('d', "debug", "debug logging", PolandBall::ValueType::BOOL);
    arguments.addArgument('D', "data", "game data directory", PolandBall::ValueType::STRING);

    if (!arguments.parse(argc, argv)) {
        return EXIT_FAILURE;
    }

    if (arguments.isSet("help") || arguments.isSet("version")) {
        return EXIT_SUCCESS;
    }

    auto& config = Graphene::GetEngineConfig();
    config.setFov(arguments.isSet("fov") ? stof(arguments.getOption("fov")) : 75.0f);
    config.setHeight(arguments.isSet("height") ? stoi(arguments.getOption("height")) : 600);
    config.setWidth(arguments.isSet("width") ? stoi(arguments.getOption("width")) : 800);
    config.setSamples(arguments.isSet("samples") ? stoi(arguments.getOption("samples")) : 0);
    config.setMaxFps(arguments.isSet("fps") ? stof(arguments.getOption("fps")) : 0.0f);
    config.setVsync(arguments.isSet("vsync"));
    config.setDebug(arguments.isSet("debug"));
    config.setDataDirectory(arguments.isSet("data") ? arguments.getOption("data") : POLANDBALL_DATADIR);

    PolandBall::PolandBall polandball;
    return polandball.exec();
}
