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
    }
    curl_global_cleanup();
}

/*!
 * \brief Uploads a file to the remote audio API.
 * \param filePath The path of the file to upload.
 * \return An optional error message. std::nullopt on success, error message on failure.
 */
std::optional<std::string> AudioAPIHandler::uploadFile(std::filesystem::path const& filePath)
{
    if (!m_curl) {
        return "cURL not initialized";
    }

    curl_mime *mime;
    curl_mimepart *part;

    mime = curl_mime_init(m_curl);

    part = curl_mime_addpart(mime);

    curl_mime_name(part, "file");
    curl_mime_filedata(part, filePath.c_str());

    curl_easy_setopt(m_curl, CURLOPT_MIMEPOST, mime);

    curl_easy_setopt(m_curl, CURLOPT_URL, (m_apiUrl + "/upload").c_str());

    CURLcode res = curl_easy_perform(m_curl);

    curl_mime_free(mime);

    return {};
}

/*!
 * \brief Downloads a file from the remote audio API.
 * \param remotePath The remote path of the file on the audio API.
 * \param localPath The local path to save the downloaded file.
 * \return An optional error message. std::nullopt on success, error message on failure.
 */
std::optional<std::string> AudioAPIHandler::downloadFile(std::string const& remotePath, std::filesystem::path const& localPath)
{
    if (!m_curl) {
        return "cURL not initialized";
    }

    curl_easy_setopt(m_curl, CURLOPT_URL, (m_apiUrl + remotePath).c_str());
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    std::ofstream out(localPath);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &out);

    CURLcode res = curl_easy_perform(m_curl);

    out.close();

    return {};
}

/*!
 * \brief The write callback function for cURL.
 * \param contents The data to write.
 * \param size The size of one data item.
 * \param nmemb The number of data items.
 * \param userp The user-defined pointer passed to the write callback.
 * \return The total size of data written.
 */
size_t AudioAPIHandler::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    auto* out = reinterpret_cast<std::ofstream*>(userp);
    size_t total_size = size * nmemb;
    out->write(reinterpret_cast<char*>(contents), total_size);
    return total_size;
}
