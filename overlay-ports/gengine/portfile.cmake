vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO YGNI-RType/GEngine
    REF v0.6.0
    SHA512 cb793c9ff630ada6392d75ab938e3ab83b7c1866ad1c6bc88ea671cdaf739b9d06c237cbd34fecdf63246170ef01f8eb3d33c3a4b1eee6b6fedf09586f1f06f0
)

set(ENV{NOT_INSTALL_VCPKG} true)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(PACKAGE_NAME "GEngine")

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)

configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)
