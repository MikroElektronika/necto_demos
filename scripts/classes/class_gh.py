import os, requests

class repo():
    class Colors:
        HEADER = '\033[95m'
        OKBLUE = '\033[94m'
        OKCYAN = '\033[96m'
        OKGREEN = '\033[92m'
        WARNING = '\033[93m'
        FAIL = '\033[91m'
        ENDC = '\033[0m'
        BOLD = '\033[1m'
        UNDERLINE = '\033[4m'

    def __init__(self, repo, token, release_id='latest'):
        self.repo = repo
        self.token = token
        self.tag = release_id if release_id else 'latest'
        self.release = self.fetch_release()
        self.assets = self.fetch_all_assets()

    def headers(self, download=False, content_type=None):
        headers = {
            'Authorization': f'token {self.token}',
            'User-Agent': 'NECTO-demos-release-script'
        }
        if download:
            headers['Accept'] = 'application/octet-stream'
        if content_type:
            headers['Content-Type'] = content_type
        return headers

    def fetch_release(self):
        """ Fetch release info either for 'latest' or for a specific tag. """
        if 'latest' == self.tag:
            url = f'https://api.github.com/repos/{self.repo}/releases/latest'
        else:
            url = f'https://api.github.com/repos/{self.repo}/releases/tags/{self.tag}'
        response = requests.get(url, headers=self.headers(), timeout=30)
        response.raise_for_status()
        release = response.json()
        print(f"{self.Colors.OKBLUE}Using release: {release['tag_name']} (id {release['id']}){self.Colors.ENDC}")
        return release

    def fetch_all_assets(self):
        """ Fetch all release assets (paginated). """
        assets = []
        page = 1
        while True:
            url = f"https://api.github.com/repos/{self.repo}/releases/{self.release['id']}/assets?page={page}&per_page=100"
            response = requests.get(url, headers=self.headers(), timeout=30)
            response.raise_for_status()
            page_assets = response.json()
            if not page_assets:
                break
            assets.extend(page_assets)
            page += 1
        return assets

    def fetch_asset(self, asset_name):
        for asset in self.assets:
            if asset['name'] == asset_name:
                return asset
        return None

    def asset_download_json(self, asset_name):
        """ Download a JSON asset and return parsed content, or None if asset does not exist. """
        asset = self.fetch_asset(asset_name)
        if not asset:
            return None
        response = requests.get(asset['url'], headers=self.headers(download=True), timeout=60)
        response.raise_for_status()
        return response.json()

    def asset_delete(self, asset_name):
        asset = self.fetch_asset(asset_name)
        if not asset:
            return
        print(f"{self.Colors.WARNING}Deleting existing asset: {asset_name}{self.Colors.ENDC}")
        response = requests.delete(asset['url'], headers=self.headers(), timeout=60)
        response.raise_for_status()
        self.assets = [a for a in self.assets if a['id'] != asset['id']]

    def asset_upload(self, asset_path, replace=True):
        """ Upload a release asset. If an asset with the same name exists, it is replaced. """
        asset_name = os.path.basename(asset_path)

        if replace:
            self.asset_delete(asset_name)

        if asset_name.endswith('.zip'):
            content_type = 'application/zip'
        elif asset_name.endswith('.json'):
            content_type = 'application/json'
        else:
            content_type = 'application/octet-stream'

        url = f"https://uploads.github.com/repos/{self.repo}/releases/{self.release['id']}/assets"
        print(f"{self.Colors.OKBLUE}Uploading asset: {asset_name}{self.Colors.ENDC}")
        with open(asset_path, 'rb') as file:
            response = requests.post(
                url, params={'name': asset_name},
                headers=self.headers(content_type=content_type),
                data=file, timeout=300
            )
        response.raise_for_status()
        asset = response.json()
        self.assets.append(asset)
        print(f"{self.Colors.OKGREEN}Uploaded asset: {asset_name} -> {asset['browser_download_url']}{self.Colors.ENDC}")

        return asset
