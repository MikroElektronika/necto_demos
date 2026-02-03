include(mikroeUtilsCommon)

lvgl_version_fetch(version)

set(PROJECT_TYPE "lvgl_designer" CACHE STRING "" FORCE)

add_subdirectory(
    ${version}
)
