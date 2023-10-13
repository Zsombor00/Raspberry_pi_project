/*!
 * \file AudioAPIHandler.cpp
 * \brief Implementation of the AudioAPIHandler class.
 */

#include "AudioAPIHandler.hpp"

/*!
 * \brief Constructs an AudioAPIHandler with a specified API URL.
 * \param URL The URL of the audio API.
 */
AudioAPIHandler::AudioAPIHandler(std::string const& URL)
    : m_apiUrl{URL}
    , m_curl(curl_easy_init())
{
    if (!m_curl) {
        throw std::runtime_error{"Failed to initialize cURL"};
    }
}

/*!
 * \brief Destructs the AudioAPIHandler, ensuring cURL is cleaned up.
 */
AudioAPIHandler::~AudioAPIHandler()
{
    if (m_curl) {
        curl_easy_cleanup(m_curl);
        curl_global_cleanup();
    }
}

/*!
 * \brief Uploads a file to the remote audio API.
 * \param inputFilePath The path of the file to upload.
 * \param outputFilePath The path of the output file.
 * \return An optional error message. std::nullopt on success, error message on failure.
 */
std::optional<std::string> AudioAPIHandler::uploadFile(
    std::filesystem::path const& inputFilePath,
    std::filesystem::path const& outputFilePath
) const
{
    if (!m_curl) {
        return "cURL not initialized";
    }

    auto mime = curl_mime_init(m_curl);
    if (!mime) {
        return "Failed to initialize mime";
    }

    auto part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        return "Failed to add mime part";
    }

    curl_mime_name(part, "file");
    curl_mime_filedata(part, inputFilePath.c_str());

    curl_easy_setopt(m_curl, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(m_curl, CURLOPT_URL, m_apiUrl.c_str());

    FILE *fp = fopen(outputFilePath.c_str(), "wb");
    if (!fp) {
        perror("fopen");
        curl_mime_free(mime);
        return "Failed to open output file";
    }

    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, fp);

    CURLcode res = curl_easy_perform(m_curl);
    fclose(fp);

    if (res != CURLE_OK) {
        curl_mime_free(mime);
        return curl_easy_strerror(res);
    }

    curl_mime_free(mime);
    return std::nullopt;
}

/*!
 * \brief The write callback function for cURL.
 * \param contents The data to write.
 * \param size The size of one data item.
 * \param nmemb The number of data items.
 * \param userp The user-defined pointer passed to the write callback.
 * \return The total size of data written.
 */
size_t AudioAPIHandler::writeCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    return fwrite(contents, size, nmemb, static_cast<FILE*>(userp));
}
